
#include "tabPageAllSource.h"

TabPageAllSource::TabPageAllSource( QWidget *parent):QWidget( parent)
{
    label_all_source = new QLabel(tr( "All Resource Items In Internet"));
    button_disp_all_source = new QPushButton( tr("Get Rresource Infomation"));
    rbutton_get_source = new QRadioButton( tr("Auto Get"));
    rbutton_get_source -> setChecked( false);
    table_all_source = new QTableWidget;
    table_all_source->setEditTriggers ( QAbstractItemView::NoEditTriggers );


    //set the horizontalheader width
    //table_all_source->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    table_all_source->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
    table_all_source->horizontalHeader()->setStretchLastSection(true);

    /* set table_all_source list */
    table_all_source -> setColumnCount( 5);
    table_all_source -> setHorizontalHeaderItem( 0, new QTableWidgetItem( tr("Name")));
    table_all_source -> setHorizontalHeaderItem( 1, new QTableWidgetItem( tr("Size")));
    table_all_source -> setHorizontalHeaderItem( 2, new QTableWidgetItem( tr("Type")));
    table_all_source -> setHorizontalHeaderItem( 3, new QTableWidgetItem( tr("Peer Amount")));
    table_all_source -> setHorizontalHeaderItem( 4, new QTableWidgetItem( tr("ID")));

    //reset the width
    table_all_source -> resizeColumnsToContents();
    table_all_source->horizontalHeader()->setStretchLastSection(true);

    QHBoxLayout  *up_layout = new QHBoxLayout;
    up_layout -> addWidget( label_all_source);
    up_layout -> addStretch();
    up_layout -> addWidget( rbutton_get_source);
    up_layout -> addWidget( button_disp_all_source);

    QVBoxLayout *down_layout = new QVBoxLayout;
    down_layout -> addWidget( table_all_source);
    //down_layout -> addStretch();

    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout -> addLayout( up_layout);
    main_layout -> addLayout( down_layout);
    //main_layout -> addStretch();

    setLayout( main_layout);
    setWindowTitle( tr("All Source"));

    connect( table_all_source, SIGNAL( cellClicked( int , int )),\
             this, SLOT( click_cell( int, int)));
    connect( rbutton_get_source, SIGNAL( clicked()),\
             this, SLOT( click_rbutton_get_source()));
    connect( button_disp_all_source, SIGNAL( clicked()),\
             this, SLOT( click_button_disp_all_source()));
    connect( &update_timer, SIGNAL( timeout()),\
             this, SLOT( click_button_disp_all_source()));

    resource_update_able = true;
}


void TabPageAllSource::click_rbutton_get_source()
{
    if( rbutton_get_source -> isChecked())
    {
        /* open the automatic getting resource function
         * by setting timer
         */
        update_timer.start( AUTO_UPDATE_RECICLE);
        button_disp_all_source -> setEnabled( false);
    }
    else
    {
        /* close the automatic getting resource function */
        update_timer.stop();
        button_disp_all_source -> setEnabled( true);
    }
}

void TabPageAllSource::click_button_disp_all_source()
{
    if(resource_update_able)
    {
        qDebug()<<"net ask for update";
        emit( update_net_resource());
    }
    resource_update_able =false;
}
void TabPageAllSource::net_res_list_update_handler(QObject *m1, QObject *m2)
{
    mresid2peer = (t_resid2peer_map *)m1;
    mresid2pro = (t_resid2propertie *)m2;
    table_all_source->setRowCount(mresid2pro->size());

    int cur_row_num = 0;
    t_resid2propertie::const_iterator it = mresid2pro->begin();
    while( it!= mresid2pro->end())
    {
        md5type id = it.key();
        resource_propertie pro = it.value().second;


        QTableWidgetItem *new_item = new QTableWidgetItem(pro.name);
        new_item ->setTextAlignment( Qt::AlignHCenter | Qt::AlignVCenter);
        table_all_source -> setItem( cur_row_num, 0, new_item);

        new_item = new QTableWidgetItem( QString::number(pro.size));
        new_item ->setTextAlignment( Qt::AlignHCenter | Qt::AlignVCenter);
        table_all_source -> setItem( cur_row_num, 1, new_item);

        new_item = new QTableWidgetItem( pro.type);
        new_item ->setTextAlignment( Qt::AlignHCenter | Qt::AlignVCenter);
        table_all_source -> setItem( cur_row_num, 2, new_item);

        new_item = new QTableWidgetItem( QString::number(it.value().first));
        new_item ->setTextAlignment( Qt::AlignHCenter | Qt::AlignVCenter);
        table_all_source -> setItem( cur_row_num, 3, new_item);

        new_item = new QTableWidgetItem( id.toHex().toUpper().data());
        new_item ->setTextAlignment( Qt::AlignHCenter | Qt::AlignVCenter);
        table_all_source -> setItem( cur_row_num, 4, new_item);

        it++;
        ++cur_row_num;
    }
    //reset the width
    table_all_source -> resizeColumnsToContents();
    table_all_source->horizontalHeader()->setStretchLastSection(true);

qDebug()<<"Updated ********";
    resource_update_able = true;
}

void TabPageAllSource::click_cell( int row, int col)
{
    PeersList   *list = new PeersList;
    list -> show();

    t_resid2peer_map::const_iterator it = mresid2peer->begin();
    for(int i = 0; i < row; i++)
    {
        /* find the place of current data */
        it++;
    }
    QList<t_ip_port> value_list = mresid2peer->values( it.key());
    qDebug()<<it.key().toHex();
    int row_num = value_list.size();

    list -> table_list -> setRowCount( row_num);

    /* insert content in the table */
    for(int i = 0; i < row_num; i++)
    {
        /* insert IP address */
        QTableWidgetItem *new_item = new QTableWidgetItem( QHostAddress( value_list.at(i).first).toString());
        new_item ->setTextAlignment( Qt::AlignHCenter | Qt::AlignVCenter);
        list -> table_list -> setItem( i, 0, new_item);
        /* insert Port */
        new_item = new QTableWidgetItem( QString::number(value_list.at(i).second));
        new_item ->setTextAlignment( Qt::AlignHCenter | Qt::AlignVCenter);
        list -> table_list -> setItem( i, 1, new_item);
    }
}
