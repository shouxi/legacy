#include "resource.h"

/*
QByteArray & resource::toString()
{
    QByteArray *block = new QByteArray;
    QTextStream out(QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out<<"<id>"<<id<<"</id>\n";
    out<<

}
*/
bool resource_propertie::operator ==(const resource_propertie &res)
{
    return true;
}
bool resource_propertie::operator <=(const resource_propertie &res)
{
    return true;
}
bool resource_propertie::operator <(const resource_propertie &res)
{
    return true;
}
bool resource_propertie::operator >=(const resource_propertie &res)
{
    return true;
}
bool resource_propertie::operator >(const resource_propertie &res)
{
    return true;
}
QDataStream & operator<<(QDataStream & out, const resource_propertie & res)
{
    out<<res.size<<res.name<<res.type<<res.other;
    return out;
}
QDataStream & operator>>(QDataStream & in, resource_propertie & res)
{
    in>>res.size>>res.name>>res.type>>res.other;
    return in;
}

bool resource::operator ==(const resource &res)
{
    return id == res.id;
}
bool resource::operator <=(const resource &res)
{
    return id <= res.id;
}
bool resource::operator <(const resource &res)
{
    return id < res.id;
}

bool resource::operator >=(const resource &res)
{
    return id >= res.id;
}
bool resource::operator >(const resource &res)
{
    return id > res.id;
}

QDataStream & operator<<(QDataStream & out, const resource & res)
{
    out<<res.id<<res.propertie;
    return out;
}
QDataStream & operator>>(QDataStream & in, resource & res)
{
    in>>res.id>>res.propertie;
    return in;
}

