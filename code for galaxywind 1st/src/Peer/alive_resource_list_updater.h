#ifndef ALIVE_RESOURCE_LIST_UPDATER_H
#define ALIVE_RESOURCE_LIST_UPDATER_H

#include "include_file.h"
#include "resource.h"


class AliveListRequester:public QTcpSocket
{
    Q_OBJECT
public:
    AliveListRequester(quint32 &alive_list_version\
                       ,const t_ip_port &tracker\
                       ,t_peer2res_version_map &peer2res_version_map\
                       ,t_peer_list &offline_res_peer_list\
                       ,QObject *parent = 0);
    void start();
private slots:
    void sendRequest();
    void readMessage();
    void connectionClosedByServer();
    void errorHandler(QAbstractSocket::SocketError err);
private:
    void connectToTracker();

    quint32 &alive_list_version;
    const t_ip_port &tracker;

    t_peer2res_version_map &peer2res_version_map;
    t_peer2res_version_map new_peer2res_version_map;

    t_peer_list &offline_res_peer_list;

    quint32 nextBlockSize;

    t_peer_list alive_peer_list;


signals:
    void alivelistupdated();

};


class ResourceRequester:public QTcpSocket
{
    Q_OBJECT
public:
    ResourceRequester(t_peer2res_version_map::iterator remote_res_peer,\
                      t_peer2resid_map &peer2resid_map,\
                      t_resid2peer_map &resid2peer_map,\
                      t_resid2propertie &resid2pro_map\
                      );

    void start();
private slots:
    void sendRequest();
    void readMessage();
    void connectToResourcePeer();
    void connectionClosedByServer();
    void errorHandler(QAbstractSocket::SocketError err);

private:
    quint32 nextBlockSize;
\
    t_peer2res_version_map::iterator remote_res_peer;

    t_peer2resid_map &peer2resid_map;
    t_resid2peer_map &resid2peer_map;

    t_resid2propertie &resid2pro_map;
    t_resource_list resource_list;

    void updateResource(t_ip_port rpc);

signals:
    void net_resource_list_update(QObject*, QObject *);

};
class AliveResourceInfoUpdater:public QObject
{
    Q_OBJECT
public:
    AliveResourceInfoUpdater(t_ip_port tracker_tcp, QObject * pareant = 0);
public slots:
    void update();
private slots:
    void requestResource();
    void errorHandler_for_req_lst(QAbstractSocket::SocketError err);
    //void errorHandler_for_req_res(QAbstractSocket::SocketError err);
    void req_complete_handler();
private:

    //tracker for get alive resource peer list ,using tcp
    t_ip_port tracker;

    t_peer2res_version_map peer2res_version_map;
    t_peer_list offline_res_peer_list;
    quint32 alive_list_version;

    t_peer2resid_map peer2resid_map;
    t_resid2peer_map resid2peer_map;

    t_resid2propertie resid2pro_map;

    void eraseOffLineResource();

    qint32 req_on_air_counter;

signals:
    void net_resource_list_update(QObject *, QObject *);
    void tracker_error();

};


#endif // ALIVE_RESOURCE_LIST_UPDATER_H
