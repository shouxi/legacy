#ifndef TRACKER_H
#define TRACKER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QUdpSocket>
#include <QHostAddress>
#include <QDebug>
#include <QTimer>

#include <QList>
#include <QPair>
#include <QMap>


#include "window.h"

typedef QPair<quint32, quint16>t_ip_port;
typedef QList<t_ip_port>t_hosts_list;
typedef QMap<t_ip_port, bool>t_alive_map;

//listen for resource peer's announce
//add the alive peer to the list
//remove the died peer
//make sure the list is newest
class AliveAnnounceReciver:public QUdpSocket
{
    Q_OBJECT
public:
    AliveAnnounceReciver(t_alive_map &alive_map, bool &aliveListChanged\
                         ,quint16 port=0, QObject *parent = 0);

private slots:
    void processAliveAnnounceDatagrams();
private:
    t_alive_map &alive_map;
    bool &aliveListChanged;
};
//hand for a peer's request
class PeerRequestHandler:public QTcpSocket
{
    Q_OBJECT
public:
    PeerRequestHandler(const t_hosts_list &alive_list, \
                   quint32 version = 0, QObject *parent = 0);
private slots:
    void readRequestMessage();
    void disconnectByPeer();
private:
    void sendAliveListMessage(bool type);

    const t_hosts_list &alive_list;
    const quint32 version;
};
//tracker demo, listen for request from peers
class AlivePeerListServer:public QTcpServer
{
    Q_OBJECT
public:
    AlivePeerListServer(t_alive_map &alive_map, bool &aliveListChanged\
                        ,quint16 port=0, QObject *parent = 0);

    void incomingConnection(int socketId);
private:
    QTimer timer;
    t_alive_map &alive_map;
    bool &aliveListChanged;
    bool versionChanged;

    t_hosts_list alive_list;
    quint32 version;
signals:
    void alive_peer_amount_update(int n);
private slots:
    void updateAliveList();
};
//main class ,it stand for a tracker
class Tracker:public QObject
{
    Q_OBJECT
public:
    Tracker(QObject *parent = 0);

private:
    MainWindow window;
    AlivePeerListServer *apl_server;
    AliveAnnounceReciver *aan_reciver;

    bool aliveListChanged;
    t_alive_map alive_map;

private slots:
    void start_handler(bool type);
    void stop_handler();
};

#endif // TRACKER_H
