#ifndef TABLEPAGE_H
#define TABLEPAGE_H

#include "include_file.h"
#include "deleteSourceWindow.h"
#include "addSourceWindow.h"
#include "resource.h"

class TabPageLocalSource : public QWidget
{
    Q_OBJECT
public:
    TabPageLocalSource ( t_resource_list &resource_list,\
                         QWidget *parent = 0);
    QTableWidget    *table_local_source;
//    ~TabPageLocalSource();
private:
    QLabel          *label_local_source;
    QPushButton     *button_add_source;
    QPushButton     *button_del_source;

    t_resource_list &my_resource_list;

    void insert_to_table_end( const resource &res);

signals:
    void update_local_resource();

private slots:
    void click_button_del_source();
    void click_button_add_source();
    void delete_row( quint16);
    void add_resource( QObject * item);
};

#endif // TABLEPAGE_H
