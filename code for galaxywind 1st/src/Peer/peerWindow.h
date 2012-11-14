#ifndef PEERWINDOW_H
#define PEERWINDOW_H

#include "include_file.h"
#include "tabPageAllSource.h"
#include "tabPageLocalSource.h"
#include "resource.h"

class PeerWindow : public QWidget
{
    Q_OBJECT

public:
    PeerWindow( t_resource_list &resource_list, QWidget *parent = 0);
    //~PeerWindow();

private:
    QLabel          *label_input_ip;
    QLineEdit       *lineEdit_input_ip;
    QPushButton     *button_connect;
    QPushButton     *button_stop_connect;
    QLabel          *label_port_1;
    QSpinBox        *spinBox_port_1;
    QLabel          *label_port_2;
    QSpinBox        *spinBox_port_2;
    QTabWidget      *tabWidget;

    TabPageAllSource *tabPageAllSource;
    TabPageLocalSource *tabPageLocalSource;

    QLabel          *label_local_ip_note;
    QLabel          *label_local_ip;

    t_resource_list &my_resource_list;


    void displayLocalIP();

signals:
    void start_peer( quint32, quint16, quint16);
    void stop_peer();

    void update_local_resource();
    void update_net_resource();

    void net_resource_list_update(QObject *, QObject*);

private slots:
    void click_connect();
public slots:
    void click_stop();
};

#endif // PEERGUI_H
