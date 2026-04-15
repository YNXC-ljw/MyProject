#include "recbox.h"
#include "ui_recbox.h"

#include "recboxitem.h"

#include<QJsonObject>

RecBox::RecBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RecBox),
    row(1),
    col(4)
{
    ui->setupUi(this);

//    RecBoxItem* boxItem = new RecBoxItem();
//    ui->recListUpHLayout->addWidget(boxItem);
}

RecBox::~RecBox()
{
    delete ui;
}

void RecBox::initRecBoxUi(QJsonArray data, int row)
{
    if(2 == row)
    {
        this->row = row;
        col = 8;
    }
    else
    {
        ui->recListDown->hide();
    }
    //将传过来的data保存起来，在成员添加即可
    imageList = data;
    //在RecBox中添加RecBoxItem对象
    createRecBoxItem();
}

//在recbox中构造并添加recBoxItem对象
void RecBox::createRecBoxItem()
{
    for(int i = 0;i < col; i++)
    {
        RecBoxItem* item = new RecBoxItem();

        //给recBoxItem对象添加图片及文本
        QJsonObject obj = imageList[i].toObject();
        item->setRecText(obj.value("text").toString());

        item->setRecImage(obj.value("path").toString());

        ui->recListUpHLayout->addWidget(item);
    }
}
