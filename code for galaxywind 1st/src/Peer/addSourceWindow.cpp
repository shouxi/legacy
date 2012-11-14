#include "addSourceWindow.h"

AddResourceDialog::AddResourceDialog( QWidget *parent)
    :QDialog(parent)
{
    lbl_name = new QLabel(tr("&Name:"));
    let_name = new QLineEdit();
    lbl_name->setBuddy(let_name);

    lbl_size = new QLabel(tr("&Size:"));
    spb_size = new QSpinBox();
    lbl_size->setBuddy(spb_size);
    spb_size->setRange(0, 2147483647);

    lbl_type = new QLabel(tr("&Type:"));
    let_type = new QLineEdit();
    lbl_type->setBuddy(let_type);

    lbl_other = new QLabel(tr("&Other:"));
    let_other = new QLineEdit();
    lbl_other->setBuddy(let_other);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(lbl_name, 0, 0);
    gridLayout->addWidget(let_name, 0, 1);
    gridLayout->addWidget(lbl_size, 1, 0);
    gridLayout->addWidget(spb_size, 1, 1);
    gridLayout->addWidget(lbl_type, 2, 0);
    gridLayout->addWidget(let_type, 2, 1);
    gridLayout->addWidget(lbl_other, 3, 0);
    gridLayout->addWidget(let_other, 3, 1);



    btn_add = new QPushButton(tr("Add"));
    btn_gen_id = new QPushButton(tr("Gen ID"));
    QVBoxLayout *vly_btn = new QVBoxLayout;
    vly_btn->addStretch();
    vly_btn->addWidget(btn_add);
    vly_btn->addWidget(btn_gen_id);
    vly_btn->addStretch();

    QHBoxLayout *hly_ctrl = new QHBoxLayout;

    hly_ctrl->addLayout(gridLayout);
    hly_ctrl->addStretch();
    hly_ctrl->addLayout(vly_btn);

    lbl_id = new QLabel(tr(" ID :"));
    lbl_id_display = new QLabel();
    QHBoxLayout *hly_id = new QHBoxLayout;
    hly_id->addWidget(lbl_id);
    hly_id->addWidget(lbl_id_display);

    QVBoxLayout *vly_main = new QVBoxLayout;

    vly_main->addLayout(hly_ctrl);
    vly_main->addLayout(hly_id);
    setLayout(vly_main);
    setWindowTitle( tr("Add Source"));

    connect(btn_add, SIGNAL(clicked()), this, SLOT(addHandler()));
    connect(btn_gen_id, SIGNAL(clicked()), this, SLOT(genidHandler()));

    this->setFixedHeight(sizeHint().height());
    new_res = new resource;
    show();
}

void AddResourceDialog::genidHandler()
{
    new_res -> propertie.name = let_name->text();
    new_res -> propertie.type = let_type->text();
    new_res -> propertie.size = spb_size->value();
/*
    qDebug()<<"*********name:"<<let_name->text();
    qDebug()<<"*********type:"<<let_type->text();
    qDebug()<<"*********size:"<<spb_size->value();
*/
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out<<let_other->text();

    new_res -> propertie.other = block;

    out<<let_name->text()<<let_type->text()<<spb_size->value();

//qDebug()<<"*********ID String:"<<block.toHex();

    new_res -> id = QCryptographicHash::hash ( block, QCryptographicHash::Md5 );

    lbl_id_display->setText(new_res -> id.toHex().toUpper());
}

void AddResourceDialog::addHandler()
{
    genidHandler();
    emit res_add( ( QObject *)new_res);
    deleteLater();
    //destroy(this);
}
