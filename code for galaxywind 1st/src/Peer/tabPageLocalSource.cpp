
#include "tabPageLocalSource.h"

TabPageLocalSource::TabPageLocalSource\
( t_resource_list &resource_list,QWidget *parent)
    : QWidget( parent),my_resource_list(resource_list)\
{
    label_local_source = new QLabel( tr("Local Resource Items List"));
    button_add_source = new QPushButton( tr("Add New Iterm"));
    button_del_source = new QPushButton( tr("Delete A Iterm"));
    table_local_source = new QTableWidget;

    //disable the gui editer
    table_local_source->setEditTriggers ( QAbstractItemView::NoEditTriggers );


    //set the horizontalheader width
    //table_local_source->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    table_local_source->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
    table_local_source->horizontalHeader()->setStretchLastSection(true);

    /* set table_local_source list */
    table_local_source -> setColumnCount( 4);
    //table_local_source -> setRowCount( 1);
    table_local_source -> setHorizontalHeaderItem( 0, new QTableWidgetItem( tr("Name")));
    table_local_source -> setHorizontalHeaderItem( 1, new QTableWidgetItem( tr("Size")));
    table_local_source -> setHorizontalHeaderItem( 2, new QTableWidgetItem( tr("Type")));
    table_local_source -> setHorizontalHeaderItem( 3, new QTableWidgetItem( tr("ID")));
    //reset the width
    //table_local_source -> resizeColumnsToContents();


    QHBoxLayout  *up_layout = new QHBoxLayout;
    up_layout -> addWidget( label_local_source);
    up_layout -> addStretch();
    up_layout -> addWidget( button_add_source);
    up_layout -> addWidget( button_del_source);

    QVBoxLayout *down_layout = new QVBoxLayout;
    down_layout -> addWidget( table_local_source);
    //down_layout -> addStretch();

    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout -> addLayout( up_layout);
    main_layout -> addLayout( down_layout);
    //main_layout -> addStretch();

    setLayout( main_layout);
    setWindowTitle( tr("Local Source"));

    /* connect all the signals and slots */
    connect( button_del_source, SIGNAL( clicked()),\
            this, SLOT( click_button_del_source()));
    connect( button_add_source, SIGNAL( clicked()),\
            this, SLOT( click_button_add_source()));
}

void TabPageLocalSource::click_button_del_source()
{
    DeleteWindow *deletewindow = new DeleteWindow(this);
    deletewindow -> show();
    connect( deletewindow,SIGNAL(delete_source_signal(quint16)),\
             this, SLOT(delete_row(quint16)));

}

void TabPageLocalSource::click_button_add_source()
{
    AddResourceDialog *add_dialog = new AddResourceDialog();
    connect( add_dialog, SIGNAL(res_add(QObject*)),\
             this, SLOT(add_resource(QObject*)));
}

void TabPageLocalSource::insert_to_table_end(const resource &res)
{
    int cur_row_num = table_local_source->rowCount();
    /*qDebug()<<cur_row_num<<res.propertie.name<<\
              res.propertie.size<<res.propertie.type<<res.id.toHex();
    */
    table_local_source -> setRowCount( cur_row_num + 1);
    //qDebug()<<"row_length()"<<table_local_source->rowCount();



    QTableWidgetItem *new_item = new QTableWidgetItem(res.propertie.name);
    new_item ->setTextAlignment( Qt::AlignHCenter | Qt::AlignVCenter);
    table_local_source -> setItem( cur_row_num, 0, new_item);

    new_item = new QTableWidgetItem( QString::number(res.propertie.size));
    new_item ->setTextAlignment( Qt::AlignHCenter | Qt::AlignVCenter);
    table_local_source -> setItem( cur_row_num, 1, new_item);

    new_item = new QTableWidgetItem( res.propertie.type);
    new_item ->setTextAlignment( Qt::AlignHCenter | Qt::AlignVCenter);
    table_local_source -> setItem( cur_row_num, 2, new_item);

    new_item = new QTableWidgetItem( res.id.toHex().toUpper().data());
    new_item ->setTextAlignment( Qt::AlignHCenter | Qt::AlignVCenter);
    table_local_source -> setItem( cur_row_num, 3, new_item);

    //reset the width
    //table_local_source -> resizeColumnsToContents();

    table_local_source -> resizeColumnsToContents();
    table_local_source->horizontalHeader()->setStretchLastSection(true);


}


void TabPageLocalSource::delete_row( quint16 n)
{

    qDebug()<<"n: "<<n;
    n--;
    if( n >= my_resource_list.size())
    {
        QMessageBox::warning(this, tr("Failed"),\
            tr("Not existed"),QMessageBox::Ok ,QMessageBox::Ok);
        return;
    }

    table_local_source -> removeRow( n);
    qDebug()<<table_local_source ->rowCount();
    my_resource_list.removeAt( n);

    emit( update_local_resource());
}

void TabPageLocalSource::add_resource(QObject *item)
{
    resource * res = ( resource*)item;
    qDebug()<<"*********add ID:"<<res->id.toHex();
    if(-1 != my_resource_list.indexOf(*res))
    {
        QMessageBox::warning(this, tr("Failed"),\
             tr("The resource existed already"),\
             QMessageBox::Ok, QMessageBox::Ok);

        delete res;
        return;
    }
    my_resource_list << (* res);
    insert_to_table_end( *res);
    delete res;
    emit( update_local_resource());
}

