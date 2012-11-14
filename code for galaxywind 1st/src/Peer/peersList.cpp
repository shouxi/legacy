
#include "peersList.h"

PeersList::PeersList( QDialog *parent):
    QDialog( parent)
{
    label_list = new QLabel( tr("Peers who have this source"));

    table_list = new QTableWidget;
    table_list -> setColumnCount( 2);
    table_list -> setEditTriggers ( QAbstractItemView::NoEditTriggers );
    table_list -> horizontalHeader()->setResizeMode(QHeaderView::Interactive);
    table_list -> horizontalHeader()->setStretchLastSection(true);
    table_list -> setHorizontalHeaderItem( 0, new QTableWidgetItem( tr("IP Address")));
    table_list -> setHorizontalHeaderItem( 1, new QTableWidgetItem( tr("Port Number")));

    QVBoxLayout *layout = new QVBoxLayout;
    layout -> addWidget( label_list);
    layout -> addWidget( table_list);

    setLayout( layout);
    setWindowTitle( tr("All Peers Who have this source"));

}
