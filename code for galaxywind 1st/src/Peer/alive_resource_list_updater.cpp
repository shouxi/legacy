#include "alive_resource_list_updater.h"


AliveListRequester::AliveListRequester(quint32 &alive_list_version\
                                       ,const t_ip_port &tracker\
                                       ,t_peer2res_version_map &peer2res_version_map\
                                       ,t_peer_list &offline_res_peer_list\
                                       ,QObject *parent)\
    :QTcpSocket(parent)\
    ,alive_list_version(alive_list_version)\
    ,tracker(tracker)\
    ,peer2res_version_map(peer2res_version_map)
    ,offline_res_peer_list(offline_res_peer_list)
{
    //qDebug()<<"a starting......\n";
    connect(this, SIGNAL(connected()), this, SLOT(sendRequest()));
    connect(this, SIGNAL(disconnected()), this , SLOT(deleteLater()));
    connect(this, SIGNAL(readyRead()), this, SLOT(readMessage()));
    connect(this, SIGNAL(disconnected()), this, SLOT(connectionClosedByServer()));

    connect(this, SIGNAL(error(QAbstractSocket::SocketError)),\
            this, SLOT(errorHandler(QAbstractSocket::SocketError)));

    nextBlockSize = 0;
}

void AliveListRequester::errorHandler(QAbstractSocket::SocketError err)
{
    qDebug()<<"TCP error for Alive List Requester,"<<err;
    deleteLater();
}

void AliveListRequester::connectionClosedByServer()
{
    qDebug()<<"shit! the server close the connection!";
}

void AliveListRequester::connectToTracker()
{
    connectToHost(QHostAddress(tracker.first), tracker.second);
    qDebug()<<"connecting to trakcer...ip:"<<tracker.first<<" port: "<<tracker.second;
}
void AliveListRequester::sendRequest()
{
    QDataStream out(this);

    out<<quint32(alive_list_version);

    qDebug()<<"send alive peer list request to tracker... local alive list version:"<<alive_list_version;
}

void AliveListRequester::readMessage()
{
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_4_7);

    if(0 == nextBlockSize)
    {
        if(bytesAvailable() < sizeof(quint32))
            return;
        in >>nextBlockSize;

        qDebug()<<"get list version:"<<alive_list_version<<", size:"<<nextBlockSize;

        if(0 == nextBlockSize)
        {
            qDebug()<<"------------------------------------------------";
            qDebug()<<"alive list is the newest,do not need to update..";
            qDebug()<<"------------------------------------------------";

            close();
            return;
        }
    }
    if(bytesAvailable() < nextBlockSize)
        return;
    in>>alive_list_version>>alive_peer_list;

    close();

    new_peer2res_version_map.clear();

    for(int i=0; i < alive_peer_list.size(); i++)
    {
        const t_ip_port &pc = alive_peer_list.at(i);
        t_peer2res_version_map::iterator it = peer2res_version_map.find(pc);
        if(it!=peer2res_version_map.end())
        {
            new_peer2res_version_map.insert(pc, it.value());
            peer2res_version_map.erase(it);
        }
        else
        {
            new_peer2res_version_map.insert(pc, 0);
        }
    }
    offline_res_peer_list = peer2res_version_map.keys();
    peer2res_version_map = new_peer2res_version_map;

    new_peer2res_version_map.clear();
}

void AliveListRequester::start()
{
    connectToTracker();
}



ResourceRequester::ResourceRequester(t_peer2res_version_map::iterator remote_res_peer,\
                                     t_peer2resid_map &peer2resid_map,\
                                     t_resid2peer_map &resid2peer_map,\
                                     t_resid2propertie &resid2pro_map\
                                     )\
    :remote_res_peer(remote_res_peer)\
    ,peer2resid_map(peer2resid_map)\
    ,resid2peer_map(resid2peer_map)\
    ,resid2pro_map(resid2pro_map)\
{
    //qDebug()<<"a starting......\n";
    connect(this, SIGNAL(connected()), this, SLOT(sendRequest()));
    connect(this, SIGNAL(disconnected()), this , SLOT(deleteLater()));
    connect(this, SIGNAL(readyRead()), this, SLOT(readMessage()));
    connect(this, SIGNAL(disconnected()), this , SLOT(connectionClosedByServer()));
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)),\
            this, SLOT(errorHandler(QAbstractSocket::SocketError)));
    nextBlockSize = 0;
}

void ResourceRequester::connectToResourcePeer()
{
    t_ip_port peer = remote_res_peer.key();
    connectToHost(QHostAddress(peer.first), peer.second);
    qDebug()<<"connecting to resource peer... ip:"<<peer.first<<"port:"<<peer.second;
}
void ResourceRequester::errorHandler(QAbstractSocket::SocketError err)
{
    qDebug()<<"TCP Errror when request resouce:"<<err;
    deleteLater();
}

void ResourceRequester::sendRequest()
{
    QDataStream out(this);
    out<<quint32(remote_res_peer.value());
    //qDebug()<<"send version request to resource peer... version:"<<remote_res_peer.value();
}
void ResourceRequester::connectionClosedByServer()
{
    qDebug()<<"shit! the resource server close the connection!";
}
void ResourceRequester::readMessage()
{
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_4_7);
    quint32 new_version;
    if(nextBlockSize == 0)
    {
        if(bytesAvailable() < sizeof(quint32))
            return;
        in >>nextBlockSize;

        if(0 == nextBlockSize)
        {
            qDebug()<<"-----------------------------------------------";
            qDebug()<<"reource  is the newest,do not need to update..";
            qDebug()<<"-----------------------------------------------";

            close();
            return;
        }
    }
    if(bytesAvailable() < nextBlockSize)
        return;
    resource_list.clear();
    in>>new_version>>resource_list;

    qDebug()<<"remote resource version change:"<<remote_res_peer.value()
           <<"->"<<new_version;

    remote_res_peer.value() = new_version;
/*
qDebug()<<"----------------------------------------------------------------";
qDebug()<<"----------------------------------------------------------------";
    for(int i = 0; i < resource_list.size(); i++)
    {
        qDebug()<<">>>>resource id:"<<resource_list.at(i).id.toHex();
    }
qDebug()<<"----------------------------------------------------------------";
qDebug()<<"----------------------------------------------------------------";
*/
    t_ip_port rpc(peerAddress().toIPv4Address(), peerPort());
    close();
    updateResource(rpc);

    emit net_resource_list_update((QObject *)&resid2peer_map, (QObject*)&resid2pro_map);
}
void ResourceRequester::updateResource(t_ip_port rpc)
{
    /*
    qDebug()<<"----------------------------------------------------------------";
    qDebug()<<"update for resource server, ip"<<rpc.first<<" port:"<<rpc.second;
    qDebug()<<"----------------------------------------------------------------";
*/
    t_resid_list pre_resid_list = peer2resid_map.values(rpc);

    for(int i=0; i < resource_list.size(); i++)
    {
        const resource &res = resource_list.at(i);
        int index =  pre_resid_list.indexOf(res.id);
        if(-1 == index)
        {
            resid2peer_map.insert(res.id, rpc);
            peer2resid_map.insert(rpc, res.id);
            qDebug()<<"insert to resid2peer map and peer2resid map";
            if(!resid2pro_map.contains(res.id))
            {
                resid2pro_map.insert(res.id, t_quote_resource(1, res.propertie));
                qDebug()<<"add to resid2pro map";
            }
            else
            {
                t_resid2propertie::iterator it = resid2pro_map.find(res.id);

                it.value().first++;
            }
        }
        else
        {
            pre_resid_list.removeAt(index);
        }
    }

    for(int i = 0; i < pre_resid_list.size(); i++)
    {
        const md5type &resid =  pre_resid_list.at(i);

        resid2peer_map.remove(resid, rpc);
        peer2resid_map.remove(rpc, resid);
        qDebug()<<"remove from resid2peer map and peer2resid map";
        if(1 == resid2pro_map.value(resid).first)
        {
            qDebug()<<"delete old from resid2pro map";
            resid2pro_map.remove(resid);
        }
        else
        {
            t_resid2propertie::iterator it = resid2pro_map.find(resid);
            it.value().first--;
        }
    }

}
void ResourceRequester::start()
{
    connectToResourcePeer();
}

AliveResourceInfoUpdater::AliveResourceInfoUpdater(t_ip_port tracker, QObject *parent)\
    : QObject(parent), tracker(tracker)
{
    peer2res_version_map.clear();
    alive_list_version = 0;
    peer2resid_map.clear();
    resid2peer_map.clear();

    resid2pro_map.clear();

    req_on_air_counter = 0;
}

void AliveResourceInfoUpdater::update()
{
    qDebug()<<"-------------------------------------";
    qDebug()<<"Alive Resource Info Updateing....";
    qDebug()<<"-------------------------------------";

    AliveListRequester *aliverequester\
            = new AliveListRequester(alive_list_version \
                                     ,tracker \
                                     ,peer2res_version_map \
                                     ,offline_res_peer_list);
    connect(aliverequester, SIGNAL(error(QAbstractSocket::SocketError)), \
            this, SLOT(errorHandler_for_req_lst(QAbstractSocket::SocketError)));
    connect(aliverequester,  SIGNAL(destroyed()), \
            this, SLOT(requestResource()));

    aliverequester->start();
}
void AliveResourceInfoUpdater::errorHandler_for_req_lst(QAbstractSocket::SocketError err)
{
    qDebug()<<"TCP Error When requset alive resource list:"<<err;
    QMessageBox::warning(0, tr("Error!Canot connect Tracker..."),\
            tr("canot connect theTracker, check the address plesse!"),QMessageBox::Ok ,QMessageBox::Ok);
    emit tracker_error();
}

void AliveResourceInfoUpdater::requestResource()
{
    qDebug()<<"request resource";
    if( peer2res_version_map.empty())
    {
        qDebug()<<"no resource";
        emit net_resource_list_update((QObject *)&resid2peer_map, (QObject*)&resid2pro_map);
    }

    req_on_air_counter = peer2res_version_map.size();
    t_peer2res_version_map::iterator it = peer2res_version_map.begin();
    for(; it != peer2res_version_map.end(); it++)
    {
        qDebug()<<"requst resource from peer:"<<it.key().first<<"@port:"<<it.key().second;

        //++req_on_air_counter;
        ResourceRequester *res_req = \
                new ResourceRequester(it, peer2resid_map,\
                                       resid2peer_map, resid2pro_map);
        res_req->start();
/*
        connect(res_req, SIGNAL(net_resource_list_update(QObject*,QObject*)),\
                this, SIGNAL(net_resource_list_update(QObject*,QObject*)));
                */
        connect(res_req, SIGNAL(destroyed()), \
                this, SLOT(req_complete_handler()));
    }

    eraseOffLineResource();
    /*
    qDebug()<<"************************************************************";
    qDebug()<<"   ------------------------------------------------------";
    qDebug()<<"          alive resource amount :"<<resid2pro_map.size();
    qDebug()<<"          alive peer amount(from Tracker) :"<<peer2res_version_map.size();
    qDebug()<<"          alive peer amount(from resource) :"<<peer2resid_map.size();
    qDebug()<<"          alive peer amount(from peer) :"<<resid2peer_map.size();
    qDebug()<<"   ------------------------------------------------------";
    qDebug()<<"************************************************************";
    */
}
void AliveResourceInfoUpdater::req_complete_handler()
{
    --req_on_air_counter;
    if(0 == req_on_air_counter)
    {
        qDebug()<<"zhi yuan qing qiu wan bi";
        emit net_resource_list_update((QObject *)&resid2peer_map, (QObject*)&resid2pro_map);
    }
    qDebug()<<"Peer Reqester,delete one";
}

void AliveResourceInfoUpdater::eraseOffLineResource()
{
    if(offline_res_peer_list.empty()) return;

    for(int i = 0 ; i < offline_res_peer_list.size(); i++)
    {
        const t_ip_port &rpc = offline_res_peer_list.at(i);
        t_resid_list resid_list = peer2resid_map.values(rpc);

        peer2resid_map.remove(rpc);

        for(int j = 0; j < resid_list.size(); j++)
        {
            const md5type &resid = resid_list.at(j);
            resid2peer_map.remove(resid, rpc);
            qDebug()<<"---------------------------------------------------------------";
            qDebug()<<"remove from resid2peer_map ip:"<<rpc.first<<" port:"<<rpc.second;
            qDebug()<<"---------------------------------------------------------------";
            if(1 == resid2pro_map.value(resid).first)
            {
                qDebug()<<"delete old from resid2pro map";
                resid2pro_map.remove(resid);
            }
            else
            {
                t_resid2propertie::iterator it = resid2pro_map.find(resid);
                it.value().first--;
            }
        }
    }
    offline_res_peer_list.clear();

    emit net_resource_list_update((QObject *)&resid2peer_map, (QObject*)&resid2pro_map);
}
