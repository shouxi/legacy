#ifndef RESOURCE_H
#define RESOURCE_H

#include "include_file.h"

class resource_propertie;
class resource;

typedef QByteArray md5type;

typedef QPair<quint32, quint16>t_ip_port;               //<ip, port>��ʶһ������
typedef QList<t_ip_port>t_peer_list;                   //�����б�
typedef QMap<t_ip_port, bool>t_alive_map;               //����״̬


typedef QMap<t_ip_port, quint32>t_peer2res_version_map;//��Դ�����汾��

typedef QPair<quint32, resource_propertie>t_quote_resource;//<time, propertie>
typedef QMap<md5type, t_quote_resource>t_resid2propertie;//id����Դ��ӳ��

typedef QMultiMap<t_ip_port, md5type>t_peer2resid_map;   //��������Դid��ӳ��
typedef QMultiMap<md5type, t_ip_port>t_resid2peer_map;    //��Դid��������ӳ��

typedef QList<resource>t_resource_list;                 //�Լ�ӵ�е���Դ
typedef QList<md5type>t_resid_list;


class resource_propertie
{
public:
    quint32 size;
    QString name;
    QString type;
    QByteArray other; //other information ,such as author

    bool operator ==(const resource_propertie &res);
    bool operator <=(const resource_propertie &res);
    bool operator <(const resource_propertie &res);
    bool operator >=(const resource_propertie &res);
    bool operator >(const resource_propertie &res);
};


QDataStream & operator<<(QDataStream & out, const resource_propertie & res);
QDataStream & operator>>(QDataStream & in, resource_propertie & res);

class resource
{
public:
    QByteArray id;//128bit md5
    bool operator ==(const resource &res);
    bool operator <=(const resource &res);
    bool operator <(const resource &res);
    bool operator >=(const resource &res);
    bool operator >(const resource &res);

    resource_propertie propertie;
};

QDataStream & operator<<(QDataStream & out, const resource & res);
QDataStream & operator>>(QDataStream & in, resource & res);


#endif // RESOURCE_H
