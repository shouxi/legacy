#include "peer.h"

Peer::Peer(QObject *parent)\
    :QObject(parent)
{

    peer_window =  new PeerWindow( my_resource_list);

    //start stop contrl
    connect( peer_window, SIGNAL(start_peer(quint32,quint16,quint16)),\
             this, SLOT(startPeerHandler(quint32,quint16,quint16)));
    connect( peer_window, SIGNAL(stop_peer()),\
             this, SLOT(stopPeerHander()));

    //get the newest resouce list of the whole netowrk
    connect(peer_window, SIGNAL(update_net_resource()),\
             this, SLOT(update_net_res_Handler()));
}

void Peer::startPeerHandler( quint32 ip, quint16 ann_port, quint16 req_port)
{

    my_resource_list.clear();/* clear the list */
    t_ip_port tracker_ann;
    t_ip_port tracker_req;
    tracker_ann.first = tracker_req.first = ip;
    tracker_ann.second = ann_port;
    tracker_req.second = req_port;
    qDebug()<<"*********Tracker "<<tracker_ann.second<<"@"<<tracker_ann.first;
    qDebug()<<"*********Tracker "<<tracker_req.second<<"@"<<tracker_req.first;

    resource_server = new PeerServerDemo(my_resource_list);

    //set the action for local resource list change
    connect( peer_window, SIGNAL(update_local_resource()),\
             resource_server, SLOT(local_resource_update_handler()));

    resource_list_port = resource_server->serverPort();

    alive_announce = new AliveAnnounce(my_resource_list\
                                       , resource_list_port\
                                       , tracker_ann);

    alive_resource_info_updater = new AliveResourceInfoUpdater(tracker_req);
    connect(alive_resource_info_updater, SIGNAL(net_resource_list_update(QObject*,QObject*)),\
            peer_window, SIGNAL(net_resource_list_update(QObject*,QObject*)));
    //if the connect tracker errors, then stop
    connect(alive_resource_info_updater, SIGNAL(tracker_error()),\
            peer_window, SLOT(click_stop()));

    alive_resource_info_updater->update();

    qDebug()<<"tracker ip:"<<tracker_ann.first;
    qDebug()<<"ann_port:"<<tracker_ann.second<<" req_port:"<<tracker_req.second;

}

void Peer::stopPeerHander()
{
    resource_server->deleteLater();
    alive_announce->deleteLater();
    alive_resource_info_updater->deleteLater();
}

void Peer::update_net_res_Handler()
{
    alive_resource_info_updater->update();

    qDebug()<<"***************need to update net resource***************";
}
