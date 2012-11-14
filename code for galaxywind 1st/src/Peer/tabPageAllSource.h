#ifndef TABPAGEALLSOURCE_H
#define TABPAGEALLSOURCE_H

#include "include_file.h"
#include "resource.h"
#include "peersList.h"

class TabPageAllSource : public QWidget
{
    Q_OBJECT
public:
    TabPageAllSource( QWidget *parent = 0);
    QTableWidget    *table_all_source;
    //~TabPageAllSource();
private:
    QLabel          *label_all_source;
    QPushButton     *button_disp_all_source;
    QRadioButton    *rbutton_get_source;
    QTimer          update_timer;

    t_resid2peer_map *mresid2peer;
    t_resid2propertie *mresid2pro;

    bool resource_update_able;

public slots:
    void net_res_list_update_handler(QObject *m1, QObject*m2);

signals:
    void update_net_resource();

private slots:
    void click_cell( int raw, int col);
    void click_rbutton_get_source();
    void click_button_disp_all_source();

};

#endif // TABPAGEALLSOURCE_H
