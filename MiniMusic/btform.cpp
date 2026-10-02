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

    setAnimation();
}

BtForm::~BtForm()
{
    delete ui;
}

void BtForm::setAnimation()
{
    //设置line1动画效果
    line1Animation = new QPropertyAnimation(ui->line1,"geometry",this);
    line1Animation->setDuration(1500); // 整个动画效果持续1500ms
    line1Animation->setKeyValueAt(0,QRect(0,15,2,0)); // 初始状态
    line1Animation->setKeyValueAt(0.5,QRect(0,0,2,15)); // 动画进行到一半(进度50%)
    line1Animation->setKeyValueAt(1,QRect(0,15,2,0)); // 动画结束，回到初始状态
    line1Animation->setLoopCount(-1); // 设置循环次数为-1，表示无限循环
    line1Animation->start();

    //设置line2动画效果
    line2Animation = new QPropertyAnimation(ui->line2,"geometry",this);
    line2Animation->setDuration(1600);
    line2Animation->setKeyValueAt(0,QRect(7,15,2,0));
    line2Animation->setKeyValueAt(0.5,QRect(7,0,2,15));
    line2Animation->setKeyValueAt(1,QRect(7,15,2,0));
    line2Animation->setLoopCount(-1);
    line2Animation->start();

    //设置line3动画效果
    line3Animation = new QPropertyAnimation(ui->line3,"geometry",this);
    line3Animation->setDuration(1700);
    line3Animation->setKeyValueAt(0,QRect(14,15,2,0));
    line3Animation->setKeyValueAt(0.5,QRect(14,0,2,15));
    line3Animation->setKeyValueAt(1,QRect(14,15,2,0));
    line3Animation->setLoopCount(-1);
    line3Animation->start();

    //设置line4动画效果
    line4Animation = new QPropertyAnimation(ui->line4,"geometry",this);
    line4Animation->setDuration(1800);
    line4Animation->setKeyValueAt(0,QRect(21,15,2,0));
    line4Animation->setKeyValueAt(0.5,QRect(21,0,2,15));
    line4Animation->setKeyValueAt(1,QRect(21,15,2,0));
    line4Animation->setLoopCount(-1);
    line4Animation->start();
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

// 显示音符跳动动画
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

// 还原鼠标离开后btform的颜色
void BtForm::clearBackground()
{
    isSelected = false;
    ui->btStyle->setStyleSheet("#btStyle:hover{ background-color: #66FFCC;}");
}

// 鼠标进入事件
void BtForm::enterEvent(QEvent *event)
{
    (void) event;
    if(!isSelected)
    {
        ui->btStyle->setStyleSheet("#btStyle:hover{ background-color: #ccffcc;}");
    }
}

// 鼠标离开事件
void BtForm::leaveEvent(QEvent *event)
{
    (void) event;
    if (isSelected) {
        // 被选中 → 保持蓝色
        ui->btStyle->setStyleSheet("#btStyle { background-color: #00ffff; }");
    } else {
        // 没被选中 → 恢复默认（透明或原来的颜色）
        ui->btStyle->setStyleSheet("#btStyle:hover{ background-color: #66FFCC;}");
    }
}

// 点击触发按钮样式并发送信号更新UI界面
void BtForm::mousePressEvent(QMouseEvent *event)
{
    // 1.鼠标按下显示其他颜色
    if(Qt::LeftButton == event->button())
    {
        isSelected = true;
        ui->btStyle->setStyleSheet("#btStyle { background-color: #00ffff;}");
    }
    // 2.按下时切换其他page页面
    emit btClicked(pageId);
}
