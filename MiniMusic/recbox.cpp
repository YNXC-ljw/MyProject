#include "recbox.h"
#include "ui_recbox.h"

#include "recboxitem.h"

RecBox::RecBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RecBox)
{
    ui->setupUi(this);

    RecBoxItem* boxItem = new RecBoxItem();
    ui->recListUpHLayout->addWidget(boxItem);
}

RecBox::~RecBox()
{
    delete ui;
}
