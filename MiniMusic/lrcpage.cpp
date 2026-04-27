#include "lrcpage.h"
#include "ui_lrcpage.h"

#include <QPushButton>

LrcPage::LrcPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LrcPage)
{
    ui->setupUi(this);

    //设置窗口无标题栏
    setWindowFlag(Qt::FramelessWindowHint);



    animation = new QPropertyAnimation(this,"geometry",this);
    animation->setDuration(400);
    animation->setStartValue(QRect(10,10,width(),height()));
    animation->setEndValue(QRect(10,10+height(),width(),height()));

    //点击下拉按钮即开启动画
    connect(ui->hideBtn,&QPushButton::clicked,this,[=]{
        animation->start();
    });

    //动画结束就隐藏起来
    connect(animation,&QPropertyAnimation::finished,this,[=]{
        hide();
    });

    ui->hideBtn->setIcon(QIcon(":/image/xiala.png"));
}

LrcPage::~LrcPage()
{
    delete ui;
}
