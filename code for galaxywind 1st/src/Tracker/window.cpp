#include "window.h"

MainWindow::MainWindow(QWidget *parent)
    :QWidget(parent)
{
   // QTextCodec::setCodecForTr(QTextCodec::codecForName("GB18030"));
    lbl_announce_listen = new QLabel(tr("Announce Listen Port:"));
    spb_announce_listen_port = new QSpinBox();
    lbl_announce_listen->setBuddy(spb_announce_listen_port);
    spb_announce_listen_port->setRange(49152, 65535);
    spb_announce_listen_port->setValue(50000);

    lbl_request_listen = new QLabel(tr("Request Listen Port:"));
    spb_request_listen_port = new QSpinBox();
    lbl_request_listen->setBuddy(spb_request_listen_port);
    spb_request_listen_port->setRange(49152, 65535);
    spb_request_listen_port->setValue(60000);


    lbl_ip_display_note = new QLabel(tr("Local IP Address"));
    lbl_ip_display = new QLabel();

    rdb_auto_port = new QRadioButton();
    rdb_auto_port->setText(tr("Use random port"));

    btn_start = new QPushButton(tr("Start"));
    btn_start->setEnabled(true);

    btn_stop= new QPushButton(tr("Stop"));
    btn_stop->setEnabled(false);

    lbl_online_res_peer = new QLabel(tr("Alive Resource Peer Amount"));

    lcd_online_res_peer_amount = new QLCDNumber();
    lcd_online_res_peer_amount->display(0);

    QHBoxLayout *hly_ann_listen = new QHBoxLayout;
    hly_ann_listen->addWidget(lbl_announce_listen);
    hly_ann_listen->addStretch();
    hly_ann_listen->addWidget(spb_announce_listen_port);

    QHBoxLayout *hly_req_listen = new QHBoxLayout;
    hly_req_listen->addWidget(lbl_request_listen);
    hly_req_listen->addStretch();
    hly_req_listen->addWidget(spb_request_listen_port);

    QHBoxLayout *hly_ip_display = new QHBoxLayout;
    hly_ip_display->addWidget(lbl_ip_display_note);
    hly_ip_display->addStretch();
    hly_ip_display->addWidget(lbl_ip_display);

    QHBoxLayout *hly_btn = new QHBoxLayout;
    hly_btn->addWidget(rdb_auto_port);
    hly_btn->addStretch();
    hly_btn->addWidget(btn_start);
    hly_btn->addWidget(btn_stop);

    QHBoxLayout *hly_lcd = new QHBoxLayout;
    hly_lcd->addWidget(lbl_online_res_peer);
    hly_lcd->addStretch();
    hly_lcd->addWidget(lcd_online_res_peer_amount);

    QVBoxLayout *main_lyt = new QVBoxLayout();

    main_lyt->addLayout(hly_ip_display);

    main_lyt->addLayout(hly_ann_listen);
    main_lyt->addLayout(hly_req_listen);

    main_lyt->addLayout(hly_btn);

    main_lyt->addLayout(hly_lcd);
    main_lyt->addStretch();

    setLayout(main_lyt);


    connect(rdb_auto_port, SIGNAL(clicked()), this, SLOT(rdb_handler()));
    connect(btn_start, SIGNAL(clicked()), this, SLOT(start_handler()));
    connect(btn_stop, SIGNAL(clicked()), this, SLOT(stop_handler()));

    displayIP();
    //setFixedSize(sizeHint().width(), sizeHint().height());
}
//handle auto
void MainWindow::rdb_handler()
{
    if(rdb_auto_port->isChecked())
    {
        spb_announce_listen_port->setEnabled(false);
        spb_request_listen_port->setEnabled(false);
    }
    else
    {
        spb_announce_listen_port->setEnabled(true);
        spb_request_listen_port->setEnabled(true);
    }
}
//handler for user's start action
void MainWindow::start_handler()
{
    btn_start->setEnabled(false);
    btn_stop->setEnabled(true);
    spb_announce_listen_port->setEnabled(false);
    spb_request_listen_port->setEnabled(false);
    rdb_auto_port->setDisabled(true);

    emit start(rdb_auto_port->isChecked());
}
//handler for user's stop action
void MainWindow::stop_handler()
{
    btn_start->setEnabled(true);
    btn_stop->setEnabled(false);
    spb_announce_listen_port->setEnabled(true);
    spb_request_listen_port->setEnabled(true);
    rdb_auto_port->setEnabled(true);

    emit stop();
}
//display the amount of resource peer ,which are online
void MainWindow::setOnlinePeerAmount(int n)
{
    lcd_online_res_peer_amount->display(n);
}

void MainWindow::setPort(quint16 udp_port,quint16 tcp_port)
{
    spb_announce_listen_port->setValue(udp_port);
    spb_request_listen_port->setValue(tcp_port);
}
//get the listen port seted by user
void MainWindow::getPort(quint16 &udp_port,quint16 &tcp_port)
{
    udp_port = spb_announce_listen_port->value();
    tcp_port = spb_request_listen_port->value();
}

//disp the local ip address
void MainWindow::displayIP()
{
    QHostInfo host = QHostInfo::fromName(QHostInfo::localHostName());
    QString ip;
    foreach (QHostAddress address, host.addresses())
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
        {
            if(!ip.isEmpty()) ip +="<br>";
            ip +=address.toString();
        }
    }
    lbl_ip_display->setText(ip);
}
