#ifndef PEERSLIST_H
#define PEERSLIST_H

#include "include_file.h"

class PeersList : public QDialog
{
    Q_OBJECT
public:
    PeersList( QDialog *parent = 0);
//    ~PeersList()
    QLabel          *label_list;
    QTableWidget    *table_list;
};

#endif // PEERSLIST_H
