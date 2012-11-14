#ifndef PEER_H
#define PEER_H

#include "include_file.h"
#include "resource.h"
#include "alive_announce.h"

#include "peer_resource_server.h"
#include "alive_resource_list_updater.h"
#include "peerWindow.h"

class Peer:public QObject
{
    Q_OBJECT
public:
    Peer(QObject *parent = 0);
//    ~Peer();

    //void addResource();
private:
    t_resource_list my_resource_list;

    quint16 resource_list_port;
    quint16 alive_announce_port;
    quint16 resource_distribute_port;

    bool resourceListChanged;
    quint32 version;

    PeerServerDemo *resource_server;
    AliveResourceInfoUpdater *alive_resource_info_updater;
    AliveAnnounce *alive_announce;

    PeerWindow    *peer_window;

private slots:
    void startPeerHandler( quint32 ip, quint16 ann_port, quint16 req_port);
    void stopPeerHander();

    void update_net_res_Handler();
};

#endif // PEER_H
