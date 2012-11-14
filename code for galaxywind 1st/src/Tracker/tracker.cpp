#include "tracker.h"

AliveAnnounceReciver::AliveAnnounceReciver(t_alive_map &alive_map\
                                           ,bool &aliveListChanged \
                                           , quint16 port\
                                           , QObject *parent)
    :QUdpSocket(parent)\
    ,alive_map(alive_map)
    ,aliveListChanged(aliveListChanged)\
{
    if(!bind(port))
    {
        qDebug()<<"AliveAnnounceReciver failed to bind to port:"<<port;
        deleteLater();
        return;
    }

    connect(this, SIGNAL(readyRead()), \
            this, SLOT(processAliveAnnounceDatagrams()));
    qDebug()<<"*************************************************************";
    qDebug()<<"AliveAnnounceReciver starting... listen@port:"<<localPort();
    qDebug()<<"*************************************************************";
}
void AliveAnnounceReciver::processAliveAnnounceDatagrams()
{
    QByteArray datagram;
    QHostAddress res_peer;
    t_ip_port new_entry;
    quint16 res_peer_port;
    do {
        datagram.resize(pendingDatagramSize());
        readDatagram(datagram.data(), datagram.size(),&res_peer);

        QDataStream in(&datagram, QIODevice::ReadOnly);
        in.setVersion(QDataStream::Qt_4_7);

        in>>res_peer_port;

        new_entry.first = res_peer.toIPv4Address();
        new_entry.second = res_peer_port;

        if(!alive_map.contains(new_entry))
        {
            aliveListChanged = true;
        }
        alive_map[new_entry]= true;
       /****
        qDebug()<<"receve alive message from:"<<res_peer.toIPv4Address()\
               <<"it's listen port:"<<res_peer_port;
       ****/
    }while (hasPendingDatagrams());
}


PeerRequestHandler::PeerRequestHandler(const t_hosts_list &alive_list, \
                               quint32 version, QObject *parent)\
    :QTcpSocket(parent),alive_list(alive_list), version(version)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(readRequestMessage()));
    connect(this, SIGNAL(disconnected()), this, SLOT(deleteLater()));
    connect(this, SIGNAL(disconnected()), this, SLOT(disconnectByPeer()));
}

void PeerRequestHandler::disconnectByPeer()
{
    qDebug()<<"-*************- disconnect by peer -************-";
}
//send alive resorce peer list to the peer, if it's necessary
void PeerRequestHandler::sendAliveListMessage(bool type)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);

    out<<quint32(0);
    if(type)
    {
        //send the version and alive resouce peer list
        out<<quint32(version)<<alive_list;
    }
    out.device()->seek(0);
    out<<quint32(block.size()-sizeof(quint32));

    write(block);
    qDebug()<<"-------------------------------------------------------------------";
    qDebug()<<"send list to host, ip:"<<peerAddress().toIPv4Address()<<" port:"<<peerPort();
}
//read peer's message
void PeerRequestHandler::readRequestMessage()
{
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_4_7);
    quint32 peer_version;

    if(bytesAvailable() < sizeof(quint32))
    {
        return;
    }

    in >> peer_version;
    qDebug()<<"get peer's version "<<peer_version<<" Tracker version:"<<version;
    sendAliveListMessage(peer_version != version);
    //close();
}

AlivePeerListServer::AlivePeerListServer(t_alive_map &alive_map\
                                         ,bool &aliveListChanged\
                                         ,quint16 port\
                                         ,QObject *parent)\
    :QTcpServer(parent)\
    ,alive_map(alive_map)\
    ,aliveListChanged(aliveListChanged)\
{
    alive_list.clear();
    alive_map.clear();
    versionChanged = false;

    if(!listen(QHostAddress::Any, port))
    {
        qDebug()<<"AlivePeerListServer Failed to bind to port:"<<port;
        deleteLater();
        return;
    }
    qDebug()<<"************************************************************";
    qDebug()<<"AlivePeerListServer starting... listen@port:"<<serverPort();
    qDebug()<<"*************************************************************";

    connect(&timer, SIGNAL(timeout()), this, SLOT(updateAliveList()));
    timer.start(4000);
}

void AlivePeerListServer::updateAliveList()
{
    t_alive_map::iterator it = alive_map.begin();

    while(it != alive_map.end())
    {
        if(!it.value())
        {
            it = alive_map.erase(it);
            aliveListChanged = true;
        }
        else
        {
            it.value() = false;
            it++;
        }
    }
    if(aliveListChanged)
    {
        aliveListChanged = false;
        versionChanged = true;

        alive_list.clear();
        alive_list = alive_map.keys();
    }
    emit alive_peer_amount_update(alive_list.size());
}


void AlivePeerListServer::incomingConnection(int socketId)
{
    if(versionChanged)
    {
        versionChanged = false;
        version++;

        qDebug()<<" alive list @version:"<<version;
        t_alive_map::iterator it = alive_map.begin();
        while(it != alive_map.end())
        {
            qDebug()<<"    peer ip:"<<it.key().first<<" port:"<<it.key().second;
            it++;
        }
    }
    PeerRequestHandler *handler = new PeerRequestHandler(alive_list, version, this);
    handler->setSocketDescriptor(socketId);
    qDebug()<<"new request for list from: "\
           <<handler->peerAddress().toIPv4Address()\
          <<" port:"<<handler->peerPort();
}


Tracker::Tracker(QObject *parent)\
    :QObject(parent)
{
    window.show();

    connect(&window, SIGNAL(start(bool)), this, SLOT(start_handler(bool)));
    connect(&window,SIGNAL(stop()), this, SLOT(stop_handler()));
}
void Tracker::start_handler(bool type)
{
    quint16 ann_port = 0;
    quint16 req_port = 0;
    aliveListChanged = false;

    if(!type)
    {
        window.getPort(ann_port, req_port);
    }
    aan_reciver = new AliveAnnounceReciver(alive_map, aliveListChanged, ann_port);
    apl_server = new AlivePeerListServer(alive_map, aliveListChanged, req_port);

    connect(apl_server, SIGNAL(alive_peer_amount_update(int)),\
            &window, SLOT(setOnlinePeerAmount(int)));

    ann_port = aan_reciver->localPort();
    req_port = apl_server->serverPort();

    window.setPort(ann_port, req_port);

}
void Tracker::stop_handler()
{
    //delete[] aan_reciver;
    //aan_reciver = NULL;
    //delete[] apl_server;
    //aan_reciver = NULL;
    aan_reciver->deleteLater();
    apl_server->deleteLater();

    qDebug()<<"-*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*--*-*--*-*--*-";
    qDebug()<<"                             STOP";
    qDebug()<<"-*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-*--*-*--*-*--*-";
}
