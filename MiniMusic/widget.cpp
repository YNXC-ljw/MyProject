#include "widget.h"
#include "ui_widget.h"
#include<QMouseEvent>
#include<QPushButton>
#include<QGraphicsDropShadowEffect>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //调用初始化方法，构造窗口
    initUi();
}

Widget::~Widget()
{
    delete ui;
}
//给窗口控制按钮设置图片
void Widget::settingBox()
{
    QPixmap pixmapskin(":/image/skin.png");
    ui->skin->setIcon(QIcon(pixmapskin));
    ui->skin->setIconSize(QSize(20,20));

    QPixmap pixmapmax(":/image/max.png");
    ui->max->setIcon(QIcon(pixmapmax));
    ui->max->setIconSize(QSize(20,20));

    QPixmap pixmapmin(":/image/min.png");
    ui->min->setIcon(QIcon(pixmapmin));
    ui->min->setIconSize(QSize(20,20));

    QPixmap pixmapquit(":/image/quit.png");
    ui->quit->setIcon(QIcon(pixmapquit));
    ui->quit->setIconSize(QSize(20,20));
}
//播放控制区按钮图片
void Widget::contralMusic()
{
    QPixmap pixmapShuffle(":/image/shuffle.png");
    ui->playModel->setIcon(QIcon(pixmapShuffle));
    ui->playModel->setIconSize(QSize(20,20));

    QPixmap pixmapUp(":/image/playUp.png");
    ui->playUp->setIcon(QIcon(pixmapUp));
    ui->playUp->setIconSize(QSize(20,20));

    QPixmap pixmapPlay(":/image/play.png");
    ui->play->setIcon(QIcon(pixmapPlay));
    ui->play->setIconSize(QSize(20,20));

    QPixmap pixmapDown(":/image/playDown.png");
    ui->playDown->setIcon(QIcon(pixmapDown));
    ui->playDown->setIconSize(QSize(20,20));

    QPixmap pixmapVolumn(":/image/volumn.png");
    ui->volume->setIcon(QIcon(pixmapVolumn));
    ui->volume->setIconSize(QSize(20,20));

    QPixmap pixmapAddLocal(":/image/add.png");
    ui->addLocal->setIcon(QIcon(pixmapAddLocal));
    ui->addLocal->setIconSize(QSize(20,20));
}
void Widget::initUi()
{
    this->setWindowFlag(Qt::FramelessWindowHint);

    //给窗口背景设置透明
    this->setAttribute(Qt::WA_TranslucentBackground);

    //给窗口设置阴影效果
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setOffset(0,0);
    shadowEffect->setColor("#000000");
    shadowEffect->setBlurRadius(15);
    this->setGraphicsEffect(shadowEffect);

    settingBox();

    contralMusic();
}


//关闭窗口按钮
void Widget::on_quit_clicked()
{
    close();
}
//窗口最小化
void Widget::on_min_clicked()
{
    this->setWindowState(Qt::WindowMinimized);
}
//窗口最大化
void Widget::on_max_clicked()
{
    this->setWindowState(Qt::WindowMaximized);
}
//鼠标点击
void Widget::mousePressEvent(QMouseEvent *event)
{
    //鼠标按下时记录鼠标坐标
    if(Qt::LeftButton == event->button())
    {
        //获取鼠标相对于屏幕左上角的坐标
         dragPosition = event->globalPos() - geometry().topLeft();
         return;
    }
    //自己实现只需要关注左键按下  其余情况交给父类函数实现
    QWidget::mousePressEvent(event);
}
//鼠标移动
void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(Qt::LeftButton == event->buttons())
    {
        //将窗口移动到相对位置
        move(event->globalPos() - dragPosition);
        return;
    }
    QWidget::mouseMoveEvent(event);
}
