#include "peer_resource_server.h"


PeerResourceSender::PeerResourceSender(const t_resource_list &resource_list,\
                       const quint32 version,\
                       QObject *parent)\
    :QTcpSocket(parent)\
    ,resource_list(resource_list)\
    ,version(version)\
{
    connect(this, SIGNAL(readyRead()), this, SLOT(readRequestMessage()));
    connect(this, SIGNAL(disconnected()), this, SLOT(deleteLater()));
}

void PeerResourceSender::readRequestMessage()
{
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_4_7);
    quint32 remote_version;

    if(bytesAvailable() < sizeof(quint32))
    {
        return;
    }

    in >> remote_version;
    qDebug()<<"Server, local version:"<<version<<" peer res version:"<<remote_version;
    sendResourceList(remote_version != version);
}
void PeerResourceSender::sendResourceList(bool type)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);

    out<<quint32(0);
    if(type)
    {
        //send to version and resource list to peer
        out<<quint32(version)<<resource_list;
/*
        qDebug()<<"send resource list to remote peer->"<<peerPort()<<"@"<<peerAddress().toIPv4Address();
        qDebug()<<"resource list, version:"<<version;
        for(int i =0 ; i< resource_list.size(); i++)
        {
            qDebug()<<"  -> "<<resource_list.at(i).id.toHex();
        }
*/
    }
    out.device()->seek(0);
    out<<quint32(block.size()-sizeof(quint32));

    write(block);
}

PeerServerDemo::PeerServerDemo(const t_resource_list &my_resource_list, \
                               QObject *parent)\
    :QTcpServer(parent)\
    ,my_resource_list(my_resource_list)\
{
    version = 1;
    listen(QHostAddress::Any);
    qDebug()<<"PeerResServer Demo Starting, listen@port:"<<serverPort();
    resVersionChanged = true;
}
void PeerServerDemo::incomingConnection(int socketId)
{
    if(resVersionChanged)
    {
        resVersionChanged = false;
        version++;
    }
    PeerResourceSender *sender = new PeerResourceSender(my_resource_list, version, this);
    sender->setSocketDescriptor(socketId);

    qDebug()<<"SERVER new resource list request from: "<<sender->peerAddress().toIPv4Address()\
           <<" port:"<<sender->peerPort();

}

void PeerServerDemo::local_resource_update_handler()
{
    resVersionChanged = true;
    qDebug()<<"Local Resource Verion Changed";
}
