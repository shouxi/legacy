

#include "deleteSourceWindow.h"

DeleteWindow::DeleteWindow( QWidget *parent):
    QDialog( parent)
{
    label_num = new QLabel( tr("Source ID: "));
    sBox_num = new QSpinBox();
    sBox_num -> setRange( 1, 65535);
    button_del = new QPushButton( tr("Delete The Reource"));

    QHBoxLayout *layout = new QHBoxLayout;
    layout -> addWidget( label_num);
    layout -> addWidget( sBox_num);
    layout -> addWidget( button_del);

    setLayout( layout);
    setWindowTitle( tr("Delete Source"));

    connect( button_del, SIGNAL(clicked()),\
             this, SLOT(click_button_del()));

    //show();
}

void DeleteWindow::click_button_del()
{
    /* judge if the input is ligel */
    emit( delete_source_signal((quint16)sBox_num->value()));
    deleteLater();
}
