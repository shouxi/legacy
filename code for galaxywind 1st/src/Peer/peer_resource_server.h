#ifndef PEER_RESOURCE_SERVER_H
#define PEER_RESOURCE_SERVER_H

#include "include_file.h"
#include "resource.h"

class PeerResourceSender:public QTcpSocket
{
    Q_OBJECT
public:
    PeerResourceSender(const t_resource_list &resource_list,\
                       const quint32 version = 0,\
                       QObject *parent = 0);

private slots:
    void readRequestMessage();

private:
    const t_resource_list &resource_list;
    const quint32 version;

    quint32 nextBlockSize;

    void sendResourceList(bool type);
};


class PeerServerDemo:public QTcpServer
{
    Q_OBJECT
public:
    PeerServerDemo(const t_resource_list &my_resource_list, \
                   QObject *parent = 0);

public slots:
    void local_resource_update_handler();

private:
    void incomingConnection(int socketId);
    const t_resource_list &my_resource_list;
    quint32 version;
    bool resVersionChanged;
};


#endif // PEER_RESOURCE_SERVER_H
