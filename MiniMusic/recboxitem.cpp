#include "recboxitem.h"
#include "ui_recboxitem.h"
#include<QDebug>
#include<QPropertyAnimation>

RecBoxItem::RecBoxItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RecBoxItem)
{
    ui->setupUi(this);

    ui->musicImageBox->installEventFilter(this);
}

RecBoxItem::~RecBoxItem()
{
    delete ui;
}
//给推荐歌设置文本
void RecBoxItem::setRecText(const QString &text)
{
    ui->recBoxItemText->setText(text);
}
//给推荐歌设置图片
void RecBoxItem::setRecImage(const QString &imagePath)
{
//    QString style = "background-image:url("+imagePath+");background-position:center center;";
//    ui->recMusicImage->setStyleSheet(style);////无法控制图片大小，如果图片大于QLabel则只显示一部分内容
    QPixmap pixmapskin(imagePath);
    ui->recMusicImage->setPixmap(pixmapskin);
    ui->recMusicImage->setScaledContents(true);//图片自适应QLabel大小
}

bool RecBoxItem::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->musicImageBox)//控件是musicImageBox
    {
        if(QEvent::Enter == event->type())//如果鼠标到达musicImageBox
        {
            //添加向上动画
            QPropertyAnimation* animal = new QPropertyAnimation(ui->musicImageBox,"geometry");
            animal->setDuration(150);
            animal->setStartValue(QRect(0,9,ui->musicImageBox->width(),ui->musicImageBox->height()));
            animal->setEndValue(QRect(0,0,ui->musicImageBox->width(),ui->musicImageBox->height()));
            animal->start();

            //对象没有挂到对象树上，动作结束时要释放避免内存泄露
            connect(animal,&QPropertyAnimation::finished,this,[=](){
                delete animal;
            });
//            qDebug() << event->type();
        }
        else if(QEvent::Leave == event->type())//离开
        {
            QPropertyAnimation* animal = new QPropertyAnimation(ui->musicImageBox,"geometry");
            animal->setDuration(150);
            animal->setStartValue(QRect(0,0,ui->musicImageBox->width(),ui->musicImageBox->height()));
            animal->setEndValue(QRect(0,9,ui->musicImageBox->width(),ui->musicImageBox->height()));
            animal->start();

            connect(animal,&QPropertyAnimation::finished,this,[=](){
                delete animal;
            });
//            qDebug() << event->type();
        }
        return true;
    }
    //其余情况由父类函数完成
    return QObject::eventFilter(watched,event);
}
