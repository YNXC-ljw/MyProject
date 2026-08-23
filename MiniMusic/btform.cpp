#include "btform.h"
#include "ui_btform.h"
#include<QMouseEvent>

BtForm::BtForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BtForm)
{
    ui->setupUi(this);

    //把音乐动画关闭，只留“本地音乐”显示
    ui->lineBox->hide();

    //设置line1动画效果
    line1Animal = new QPropertyAnimation(ui->line1,"geometry",this);
    line1Animal->setDuration(1500);
    line1Animal->setKeyValueAt(0,QRect(0,15,2,0));
    line1Animal->setKeyValueAt(0.5,QRect(0,0,2,15));
    line1Animal->setKeyValueAt(1,QRect(0,15,2,0));
    line1Animal->setLoopCount(-1);
    line1Animal->start();

    //设置line2动画效果
    line2Animal = new QPropertyAnimation(ui->line2,"geometry",this);
    line2Animal->setDuration(1600);
    line2Animal->setKeyValueAt(0,QRect(7,15,2,0));
    line2Animal->setKeyValueAt(0.5,QRect(7,0,2,15));
    line2Animal->setKeyValueAt(1,QRect(7,15,2,0));
    line2Animal->setLoopCount(-1);
    line2Animal->start();

    //设置line3动画效果
    line3Animal = new QPropertyAnimation(ui->line3,"geometry",this);
    line3Animal->setDuration(1700);
    line3Animal->setKeyValueAt(0,QRect(14,15,2,0));
    line3Animal->setKeyValueAt(0.5,QRect(14,0,2,15));
    line3Animal->setKeyValueAt(1,QRect(14,15,2,0));
    line3Animal->setLoopCount(-1);
    line3Animal->start();

    //设置line4动画效果
    line4Animal = new QPropertyAnimation(ui->line4,"geometry",this);
    line4Animal->setDuration(1800);
    line4Animal->setKeyValueAt(0,QRect(21,15,2,0));
    line4Animal->setKeyValueAt(0.5,QRect(21,0,2,15));
    line4Animal->setKeyValueAt(1,QRect(21,15,2,0));
    line4Animal->setLoopCount(-1);
    line4Animal->start();
}

BtForm::~BtForm()
{
    delete ui;
}

void BtForm::setIconAndText(const QString& btIcon,const QString& btText,int pageId)
{
    //给按钮设置图标
    ui->btIcon->setPixmap(QPixmap(btIcon));
    ui->btIcon->setScaledContents(true);   // 开启图片自适应
    ui->btIcon->setFixedSize(20, 20);
    //给按钮设置文本
    ui->btText->setText(btText);

    //将按钮和MiniMusic的page关联起来
    this->pageId = pageId;

}

int BtForm::getPageId() const
{
    return pageId;
}

void BtForm::clearBackground()
{
    ui->btStyle->setStyleSheet("#btStyle:hover{ background-color: #66FFCC;}");
}

void BtForm::showAnimal(bool isShow)
{
    if(isShow)
    {
        ui->lineBox->show();
    }
    else
    {
        ui->lineBox->hide();
    }
}

// 点击触发按钮样式并发送信号更新UI界面
void BtForm::mousePressEvent(QMouseEvent *event)
{
    // 1.鼠标按下显示其他颜色
    if(Qt::LeftButton == event->button())
    {
        ui->btStyle->setStyleSheet("#btStyle { background-color: #00ffff;}");
    }
    // 2.按下时切换其他page页面
    emit btClicked(pageId);
}
