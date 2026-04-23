#include "widget.h"
#include "ui_widget.h"
#include "btform.h"
#include "commonpage.h"

#include <QMouseEvent>
#include <QPushButton>

#include <QGraphicsDropShadowEffect>
#include <QDebug>

#include <QJsonObject>
#include <QJsonArray>

#include <QFileDialog>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //调用初始化方法，构造窗口
    initUi();

    playerInit();

    connectSignalAndSlots();


//    ui->progressBar->setStyleSheet("background-color:white;");
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
    QPixmap pixmapShuffle(":/image/random.png");
    ui->playModel->setIcon(QIcon(pixmapShuffle));
    ui->playModel->setIconSize(QSize(15,15));

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

    //响应commonPage发出的处理信号
    connect(ui->likePage,&CommonPage::updateLikeMusic,this,&Widget::updateLikeMusicAndPage);
    connect(ui->localPage,&CommonPage::updateLikeMusic,this,&Widget::updateLikeMusicAndPage);
    connect(ui->recentPage,&CommonPage::updateLikeMusic,this,&Widget::updateLikeMusicAndPage);

    //播放控制区信号处理
    connect(ui->play,&QPushButton::clicked,this,&Widget::onPlayMiusic);
    connect(ui->playUp,&QPushButton::clicked,this,&Widget::onPlayUpClicked);
    connect(ui->playDown,&QPushButton::clicked,this,&Widget::onPlayDownClicked);
    connect(ui->playModel,&QPushButton::clicked,this,&Widget::onPlayModelClicked);
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

    std::random_shuffle(vecImageName.begin(),vecImageName.end());//若没有设置随机数种子，那么每次的种子都是一样的，运行n次都是一样的结果

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

    //初始化推荐页面
    srand(time(NULL));
    ui->recMusicBox->initRecBoxUi(randomPiction(),1);
    ui->supplyMusicBox->initRecBoxUi(randomPiction(),2);

    //初始化page页面
    ui->likePage->setMusicListType(PageType::LIKE_PAGE);
    ui->likePage->setCommonPageUi("我喜欢",":/image/ilike.jpg");

    ui->localPage->setMusicListType(PageType::LOCAL_PAGE);
    ui->localPage->setCommonPageUi("本地音乐",":/image/local.jpg");

    ui->recentPage->setMusicListType(PageType::HISTORY_PAGE);
    ui->recentPage->setCommonPageUi("最近播放",":/image/recent.jpg");

    volumeTool = new VolumeTool(this);


}

void Widget::playerInit()
{
    // 1. 初始化播放相关类对象
    player = new QMediaPlayer(this);
    playerList = new QMediaPlaylist(this);

    // 2. 设置默认播放模式
    playerList->setPlaybackMode(QMediaPlaylist::Random);

    // 3. 将播放列表设置到播放媒体对象中
    player->setPlaylist(playerList);

    // 4. 设置默认音量
    player->setVolume(20);
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
    this->setWindowState(Qt::WindowFullScreen);
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

void Widget::updateLikeMusicAndPage(bool isLike, const QString &musicId)
{
    // 1. 修改状态
    auto it = musicList.findMusicById(musicId);
    if(it != musicList.end())
    {
        it->setIsLike(isLike);
    }

    // 2. 更新page页面的歌曲信息
    ui->likePage->reFrush(musicList);
    ui->localPage->reFrush(musicList);
    ui->recentPage->reFrush(musicList);
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

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//音量调节按钮
void Widget::on_volume_clicked()
{
    // 1. 获取ui->volume控件的左上角坐标，并转换为全局坐标
    QPoint point = ui->volume->mapToGlobal(QPoint(0,0));

    // 2. 计算volumeTool窗口所需要移动到的左上角的坐标
    QPoint volumeLeftTop = point - QPoint(volumeTool->width() / 2,volumeTool->height());

    volumeLeftTop.setX(volumeLeftTop.x() + 15);
    volumeLeftTop.setY(volumeLeftTop.y() + 30);
    // 3. 将volumeTool移动到坐标位置
    volumeTool->move(volumeLeftTop);

    // 4. 显示窗口
    volumeTool->show();
}

//添加本地音乐按钮
void Widget::on_addLocal_clicked()
{
    QFileDialog fileDialog(this);

    //添加本地音源
    fileDialog.setWindowTitle("添加本地音乐");

    //设置文件对话框打开的默认路径
    QDir dir(QDir::currentPath());
    dir.cdUp();
    QString projectPath = dir.path();
    projectPath += "/MiniMusic/music";
    fileDialog.setDirectory(projectPath);

    //设置一次可以选择多个文件
    fileDialog.setFileMode(QFileDialog::ExistingFiles);
//    fileDialog.exec();

    //通过MIME类型来过滤文件
    QStringList mimeTypeFilters;
    mimeTypeFilters << "application/octet-stream";
    fileDialog.setMimeTypeFilters(mimeTypeFilters);

    if(fileDialog.exec() == QDialog::Accepted)//当打开对话框并选择了文件时
    {
        //获取选中的文件
        QList<QUrl> fileUrls = fileDialog.selectedUrls();//fileUrls中存放的是被选中的所有文件的路径

        //将所有音乐添加到音乐列表中进行管理
        musicList.addMusicsByUrl(fileUrls);//存放的是已经被解析过的music

        //要将文件中的音乐上传到本地，那默认的commonPage页面就应该是"本地下载"页面
        ui->stackedWidget->setCurrentIndex(4);

        ui->localPage->reFrush(musicList);

        ui->localPage->addMusicToPlayList(musicList,playerList);
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//播放控制区

//播放歌曲按钮
void Widget::onPlayMiusic()
{
    if(QMediaPlayer::PlayingState == player->state())//播放状态，应该暂停
    {
        player->pause();
        ui->play->setIcon(QIcon(":/image/play.png"));
    }
    else if(QMediaPlayer::PausedState == player->state())//暂停状态
    {
        player->play();
        ui->play->setIcon(QIcon(":/image/play_2.png"));
    }
    else if(QMediaPlayer::StoppedState == player->state())//停止状态，直接启动即可
    {
        player->play();
        ui->play->setIcon(QIcon(":/image/play_2.png"));
    }
    else
    {
        qDebug() << player->errorString();
    }
}

void Widget::onPlayUpClicked()
{
    playerList->previous();
}

void Widget::onPlayDownClicked()
{
    playerList->next();
}

void Widget::onPlayModelClicked()
{
    //顺序播放--->随机播放--->单曲循环
    if(playerList->playbackMode() == QMediaPlaylist::Loop)//顺序播放
    {
        playerList->setPlaybackMode(QMediaPlaylist::Random);
        QIcon icon(":/image/random.png");
        QIcon fixedIcon = QIcon(icon.pixmap(15, 15)); // 强制转为 15x15 像素
        ui->playModel->setIcon(fixedIcon);
        ui->playModel->setToolTip("随机播放");
    }
    else if (playerList->playbackMode() == QMediaPlaylist::Random) //随机播放
    {
        playerList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        QIcon icon(":/image/single.png");
        QIcon fixedIcon = QIcon(icon.pixmap(18, 18));
        ui->playModel->setIcon(fixedIcon);
        ui->playModel->setToolTip("单曲循环");
    }
    else if(playerList->playbackMode() == QMediaPlaylist::CurrentItemInLoop)//单曲循环
    {
        playerList->setPlaybackMode(QMediaPlaylist::Loop);
        QIcon icon(":/image/loop.png");
        QIcon fixedIcon = QIcon(icon.pixmap(16, 16));
        ui->playModel->setIcon(fixedIcon);
        ui->playModel->setToolTip("顺序播放");
    }
    else
    {
        qDebug() << "暂不支持";
    }
}
