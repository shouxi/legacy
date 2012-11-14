#include "peerWindow.h"

class QHostInfo;


const QString MOREN_ADD = QString("192.168.1.1");


PeerWindow::PeerWindow( t_resource_list &resource_list,\
                        QWidget *parent)
    : QWidget( parent) \
     ,my_resource_list(resource_list)\
{
    label_input_ip = new QLabel( tr("Input IP Address"));
    lineEdit_input_ip = new QLineEdit( tr(""));
    lineEdit_input_ip->setText(MOREN_ADD);

    button_connect = new QPushButton( tr("Connect"));
    button_stop_connect = new QPushButton( tr("Stop"));
    button_stop_connect -> setEnabled( false);
    label_port_1 = new QLabel( tr("Announce Listen Port:"));
    spinBox_port_1 = new QSpinBox;
    spinBox_port_1 ->setRange( 0, 65535);
    spinBox_port_1 ->setValue( 50000);
    label_port_2 = new QLabel( tr("Request Listen Port:"));
    spinBox_port_2 = new QSpinBox;
    spinBox_port_2 ->setRange( 0, 65535);
    spinBox_port_2 ->setValue( 60000);
    QHBoxLayout  *up_layout = new QHBoxLayout;
    up_layout -> addWidget( label_input_ip);
    up_layout -> addWidget( lineEdit_input_ip);
    up_layout -> addWidget( label_port_1);
    up_layout -> addWidget( spinBox_port_1);
    up_layout -> addWidget( label_port_2);
    up_layout -> addWidget( spinBox_port_2);
    up_layout -> addStretch();
    up_layout -> addWidget( button_connect);
    up_layout -> addWidget( button_stop_connect);

    tabWidget = new QTabWidget;
    tabPageAllSource = new TabPageAllSource;
    tabPageLocalSource = new TabPageLocalSource( my_resource_list);
    tabWidget -> addTab( tabPageLocalSource, tr( "All Resource In Local"));
    tabWidget -> addTab( tabPageAllSource, tr( "All Resource In the Internet"));

    tabWidget->setEnabled(false);

    QHBoxLayout *down_layout = new QHBoxLayout;
    down_layout -> addWidget( tabWidget);

    label_local_ip_note = new QLabel( tr("Local IP Address: "));
    label_local_ip = new QLabel( tr("127.0.0.1"));
    QHBoxLayout *down_down_layout = new QHBoxLayout;
    down_down_layout -> addStretch();
    down_down_layout -> addWidget( label_local_ip_note);
    down_down_layout -> addWidget( label_local_ip);

    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout -> addLayout( up_layout);
    main_layout -> addLayout( down_layout);
    main_layout -> addLayout( down_down_layout);
    //main_layout -> addStretch();

    setLayout( main_layout);
    setWindowTitle( tr("Peer"));
    //setFixedHeight( sizeHint().height());

    connect( button_connect, SIGNAL( clicked()), \
             this, SLOT( click_connect()));
    connect( button_stop_connect, SIGNAL( clicked()), \
             this, SLOT( click_stop()));

    // local's resource changed emit the signal to make version change
    connect( tabPageLocalSource, SIGNAL(update_local_resource()),\
             this, SIGNAL(update_local_resource()));

    // need to update the resource list of whole network, emit the signal
    connect(tabPageAllSource, SIGNAL(update_net_resource()),\
             this, SIGNAL(update_net_resource()));

    connect(this, SIGNAL(net_resource_list_update(QObject*,QObject*)),\
            tabPageAllSource, SLOT(net_res_list_update_handler(QObject*,QObject*)));

    show();

    displayLocalIP();
}

void PeerWindow::click_connect()
{

    /* Clear both local resource and all resource lists */
    tabPageAllSource -> table_all_source ->setRowCount(0);
    /* Clear both local resource and all resource lists */
    tabPageLocalSource -> table_local_source ->setRowCount(0);

    //check the right of ip address
    QHostAddress ttr( lineEdit_input_ip -> text());
    quint32 ip = ttr.toIPv4Address();
       if(0 == ip)
       {
           QMessageBox::warning(this, tr("Address Error"),\
           tr("The Address is incorrect!"),QMessageBox::Ok ,QMessageBox::Ok);
           lineEdit_input_ip ->setText( MOREN_ADD);
           return;
       }

    /* GUI changes */
    button_connect -> setEnabled( false);
    button_stop_connect -> setEnabled( true);
    lineEdit_input_ip -> setEnabled( false);
    spinBox_port_1 -> setEnabled( false);
    spinBox_port_2 -> setEnabled( false);
    tabWidget->setEnabled(true);

    //qDebug()<<ttr.toIPv4Address()<<":"<<(quint16)spinBox_port_1->value()<<":"<<(quint16)spinBox_port_2 ->value();
    emit( start_peer(ip, \
                      (quint16)spinBox_port_1->value(),\
                      (quint16)spinBox_port_2 ->value()));

}

void PeerWindow::click_stop()
{
    qDebug()<<"Stop";
    button_connect -> setEnabled( true);
    button_stop_connect -> setEnabled( false);
    lineEdit_input_ip -> setEnabled( true);
    spinBox_port_1 -> setEnabled( true);
    spinBox_port_2 -> setEnabled( true);
    tabWidget->setEnabled(false);

    emit( stop_peer());
}

void PeerWindow::displayLocalIP()
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
    label_local_ip->setText(ip);
}
