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
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
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

    currentIndex = 0;
    count = imageList.size() / col;
    //在RecBox中添加RecBoxItem对象
    createRecBoxItem();
}

//在recbox中构造并添加recBoxItem对象
void RecBox::createRecBoxItem()
{
    //为了避免重复构造item，导致推荐框图片大于4，要删除原先已经存在的推荐图再构造
    QList<RecBoxItem*> recUpList = ui->recListUp->findChildren<RecBoxItem*>();
    for(auto e : recUpList)
    {
        ui->recListUpHLayout->removeWidget(e);
        delete e;
    }
    QList<RecBoxItem*> recDownList = ui->recListDown->findChildren<RecBoxItem*>();
    for(auto e : recDownList)
    {
        ui->recListDownVLayout->removeWidget(e);
        delete e;
    }

    int index = 0;
    for(int i = currentIndex * col;i < col + col * currentIndex; i++)
    {
        RecBoxItem* item = new RecBoxItem();

        //给recBoxItem对象添加图片及文本
        QJsonObject obj = imageList[i].toObject();
        item->setRecText(obj.value("text").toString());
        item->setRecImage(obj.value("path").toString());

        //今日为你推荐：row = 1, col = 4
        //音乐补给站：row = 2, col = 8
        if(2 == row && index >= col/2)
        {
            ui->recListDownVLayout->addWidget(item);
        }
        else
        {
            ui->recListUpHLayout->addWidget(item);
        }
        index++;
    }
}

void RecBox::on_btUp_clicked()
{
    //点击之后显示上一组的图片，如果已经是第0组了，点击后就显示最后一组
    currentIndex--;
    if(currentIndex < 0)
    {
        currentIndex = count - 1;
    }

    createRecBoxItem();
}

void RecBox::on_btDown_clicked()
{
    //点击之后显示下一组的图片，如果已经是最后一组了，点击后就显示第0组
    currentIndex++;
    if(currentIndex >= count)
    {
        currentIndex = 0;
    }

    createRecBoxItem();
}
