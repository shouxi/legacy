#ifndef WINDOW_H
#define WINDOW_H

#include <QLabel>
#include <QWidget>
#include <QSpinBox>
#include <QPushButton>
#include <QRadioButton>
#include <QLCDNumber>
#include <QTextCodec>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHostInfo>

class MainWindow:public QWidget
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    void setPort(quint16 udp_port,quint16 tcp_port);
    void getPort(quint16 &udp_port,quint16 &tcp_port);
public slots:
    void setOnlinePeerAmount(int n);
signals:
    void start(bool isAuto);
    void stop();
private slots:
    void start_handler();
    void stop_handler();
    void rdb_handler();

private:
    QLabel *lbl_announce_listen;
    QSpinBox *spb_announce_listen_port;

    QLabel *lbl_request_listen;
    QSpinBox *spb_request_listen_port;

    QLabel *lbl_ip_display_note;
    QLabel *lbl_ip_display;

    QRadioButton *rdb_auto_port;

    QPushButton *btn_start;
    QPushButton *btn_stop;

    QLabel *lbl_online_res_peer;
    QLCDNumber *lcd_online_res_peer_amount;

    void displayIP();
};

#endif // WINDOW_H
