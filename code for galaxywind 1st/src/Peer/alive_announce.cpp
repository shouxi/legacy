#include "alive_announce.h"


AliveAnnounce::AliveAnnounce(const t_resource_list &res, \
                             const quint16 &ll_port, \
                             const t_ip_port tracker )\
    :res_list(res), ll_port(ll_port), tracker(tracker)
{
     connect(&timer, SIGNAL(timeout()), this, SLOT(aliveAnnounce()));
     timer.start(2000);
}
void AliveAnnounce::aliveAnnounce()
{
    if(res_list.empty()) return;

    QByteArray datagram;
    QDataStream out(&datagram, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);

    out<<quint16(ll_port);

    //qDebug()<<"send alive message to tracker:"<<tracker.first<<":"<<tracker.second<<"listen@port:"<<ll_port;
    writeDatagram(datagram, QHostAddress(tracker.first), tracker.second);
}

