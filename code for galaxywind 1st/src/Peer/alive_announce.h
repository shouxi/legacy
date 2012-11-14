#ifndef ALIVE_ANNOUNCE_H
#define ALIVE_ANNOUNCE_H

#include "include_file.h"
#include "resource.h"

// send alive message to the tracker periodic,if it own some resource.
class AliveAnnounce:public QUdpSocket
{
    Q_OBJECT
public:
    AliveAnnounce(const t_resource_list &res, const quint16 &ll_port, const t_ip_port tracker);

private slots:
    void aliveAnnounce();

private:
    QTimer timer;
    const t_resource_list &res_list;
    const quint16 &ll_port;
    const t_ip_port tracker;
};


#endif // ALIVE_ANNOUNCE_H
