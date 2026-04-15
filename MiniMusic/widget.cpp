#include "widget.h"
#include "ui_widget.h"
#include"btform.h"

#include<QMouseEvent>
#include<QPushButton>

#include<QGraphicsDropShadowEffect>
#include<QDebug>

#include<QJsonObject>
#include<QJsonArray>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //调用初始化方法，构造窗口
    initUi();

    connectSignalAndSlots();
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
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//管理所有信号与信号槽的函数
void Widget::connectSignalAndSlots()
{
    connect(ui->Rec,&BtForm::btClicked,this,&Widget::onBtClicked);
    connect(ui->audio,&BtForm::btClicked,this,&Widget::onBtClicked);
    connect(ui->music,&BtForm::btClicked,this,&Widget::onBtClicked);
    connect(ui->like,&BtForm::btClicked,this,&Widget::onBtClicked);
    connect(ui->local,&BtForm::btClicked,this,&Widget::onBtClicked);
    connect(ui->recent,&BtForm::btClicked,this,&Widget::onBtClicked);
}
//随机推荐图片
QJsonArray Widget::randomPiction()
{
    QVector<QString> vecImageName;
    vecImageName<< "001.jpg" << "002.jpg" << "003.jpg" << "004.jpg" << "005.jpg" << "006.jpg"
                << "007.jpg" << "008.jpg" << "009.jpg" << "010.jpg" << "011.jpg" << "012.jpg"
                << "013.jpg" << "014.jpg" << "015.jpg" << "016.jpg" << "017.jpg" << "018.jpg"
                << "019.jpg" << "020.jpg" << "021.jpg" << "022.jpg" << "023.jpg" << "024.jpg"
                << "025.jpg" << "026.jpg" << "027.jpg" << "028.jpg" << "029.jpg" << "030.jpg"
                << "031.jpg" << "032.jpg" << "033.jpg" << "034.jpg" << "035.jpg" << "036.jpg"
                << "037.jpg" << "038.jpg"<< "039.jpg";

    std::random_shuffle(vecImageName.begin(),vecImageName.end());

    //path = ":/image/pages/"+vecImageName[i];
    //text = "推荐-001";

    QJsonArray objArray;
    for(int i = 0; i < vecImageName.size(); i++)
    {
        QJsonObject obj;
        obj.insert("path",":/image/pages/"+vecImageName[i]);

        QString strText = QString("推荐-%1").arg(i,3,10,QChar('0'));
        obj.insert("text",strText);

        objArray.append(obj);
    }
    return objArray;
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

    //给BtForm设置图标和文本信息
    ui->Rec->setIconAndText(":/image/rec.png","推荐", 0);
    ui->audio->setIconAndText(":/image/radio.png","电台", 1);
    ui->music->setIconAndText(":/image/music.png","音乐馆", 2);
    ui->like->setIconAndText(":/image/like.png","我喜欢", 3);
    ui->local->setIconAndText(":/image/local.png","本地和下载", 4);
    ui->recent->setIconAndText(":/image/recent.png","最近播放", 5);

    //让本地下载默认显示音符跳动
    ui->local->showAnimal();
    ui->stackedWidget->setCurrentIndex(4);

    ui->recMusicBox->initRecBoxUi(randomPiction(),1);
    ui->supplyMusicBox->initRecBoxUi(randomPiction(),2);
}
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//响应btform发出的信号（信号处理函数）
void Widget::onBtClicked(int pageId)
{
    //获取到所有btForm的按钮并清除点击后残留的颜色
    QList<BtForm*> btFormList = this->findChildren<BtForm*>();
    for(auto btForm : btFormList)
    {
        if(btForm->getPageId() != pageId)
        {
            btForm->clearBackground();
        }
    }
    ui->stackedWidget->setCurrentIndex(pageId);
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
