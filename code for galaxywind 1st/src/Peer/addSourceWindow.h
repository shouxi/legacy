#ifndef ADDSOURCEWINDOW_H
#define ADDSOURCEWINDOW_H

#include "include_file.h"
#include "resource.h"

class AddResourceDialog:public QDialog
{
    Q_OBJECT
public:
    AddResourceDialog( QWidget *parent = 0);

private:
    QLabel *lbl_name;
    QLineEdit *let_name;

    QLabel *lbl_size;
    QSpinBox *spb_size;

    QLabel *lbl_type;
    QLineEdit *let_type;

    QLabel *lbl_other;
    QLineEdit *let_other;

    QPushButton *btn_add;
    QPushButton *btn_gen_id;

    QLabel *lbl_id;
    QLabel *lbl_id_display;

    resource *new_res;

private slots:
    void addHandler();
    void genidHandler();
signals:
    void res_add( QObject *);
};



#endif // ADDSOURCEWINDOW_H
