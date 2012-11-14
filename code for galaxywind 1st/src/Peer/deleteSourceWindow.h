#ifndef DELETEWINDOW_H
#define DELETEWINDOW_H

#include "include_file.h"

class DeleteWindow : public QDialog
{
    Q_OBJECT

public:
    DeleteWindow( QWidget *parent = 0);
//    ~DeleteWindow();
private:
    QLabel          *label_num;
    QSpinBox        *sBox_num;
    QPushButton     *button_del;

signals:
    void delete_source_signal( quint16);

private slots:
    void click_button_del();
};

#endif // DELETEWINDOW_H
