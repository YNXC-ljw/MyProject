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
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QSystemTrayIcon>
#include <QMenu>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , currentIndex(-1)
{
    ui->setupUi(this);

    //调用初始化方法，构造窗口
    initUi();

    // 初始化播放类对象
    playerInit();

    // 初始化数据库
    initSqlite();

    // 将数据库中歌曲初始化到界面
    initMusicList();

    // 关联信号和槽函数
    connectSignalAndSlots();


//    ui->progressBar->setStyleSheet("background-color:white;");
}

Widget::~Widget()
{
    delete ui;
}
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//初始化模块

void Widget::initUi()
{
    this->setWindowFlag(Qt::FramelessWindowHint);

    //给窗口背景设置透明
    this->setAttribute(Qt::WA_TranslucentBackground);

    //设置任务栏图标
    this->setWindowIcon(QIcon(":/image/logotubiao.png"));

    //添加系统托盘
    QSystemTrayIcon* trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/image/logotubiao.png"));
    trayIcon->show();
    //给托盘添加菜单
    QMenu* trayMenu = new QMenu();
    trayMenu->addAction("显示",this,&QWidget::showNormal);
    trayMenu->addAction("退出",this,&Widget::onMiniMusicQuit);
    trayIcon->setContextMenu(trayMenu);

    //给窗口设置阴影效果
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setOffset(0,0);
    shadowEffect->setColor("#000000");
    shadowEffect->setBlurRadius(10);
    this->setGraphicsEffect(shadowEffect);

    ui->max->setEnabled(false);//禁用窗口最大化

    settingBox();

    contralMusic();

    //给BtForm设置图标和文本信息
    ui->Rec->setIconAndText(":/image/rec.png","推荐", 0);
    ui->audio->setIconAndText(":/image/radio.png","电台", 1);
    ui->music->setIconAndText(":/image/music.png","音乐馆", 2);
    ui->like->setIconAndText(":/image/like.png","我喜欢", 3);
    ui->local->setIconAndText(":/image/local.png","本地和下载", 4);
    ui->recent->setIconAndText(":/image/recent.png","最近播放", 5);

    //将localPage设置为默认页面
    ui->stackedWidget->setCurrentIndex(4);
    currentPage = ui->localPage;
    //让本地下载默认显示音符跳动
    ui->local->showAnimal(true);

    //初始化推荐页面
    srand(time(NULL));
    ui->recMusicBox->initRecBoxUi(randomPiction(),1);
    ui->supplyMusicBox->initRecBoxUi(randomPiction(),2);

    //初始化page页面
    ui->likePage->setCommonPageUi("我喜欢",":/image/ilike.jpg");
    ui->localPage->setCommonPageUi("本地音乐",":/image/local.jpg");
    ui->recentPage->setCommonPageUi("最近播放",":/image/recent.jpg");

    volumeTool = new VolumeTool(this);

    //实例化LrcWord对象
    lrcPage = new LrcPage(this);
    lrcPage->setGeometry(10,10,lrcPage->width(),lrcPage->height());
    lrcPage->hide();

    //初始化上移对象
    lrcPageAnimation = new QPropertyAnimation(lrcPage,"geometry",this);
    lrcPageAnimation->setDuration(400);
    lrcPageAnimation->setStartValue(QRect(10,10+lrcPage->height(),lrcPage->width(),lrcPage->height()));
    lrcPageAnimation->setEndValue(QRect(10,10,lrcPage->width(),lrcPage->height()));
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
//////////////////////////////////////////////////
    //关联QMediaPlayer的信号

    //关联QMediaPlayer::Duration信号
    connect(player,&QMediaPlayer::durationChanged,this,&Widget::onDurationChanged);

    //关联QMediaPlayer::Position信号
    connect(player,&QMediaPlayer::positionChanged,this,&Widget::onPositionChanged);

    //关联播放元数据改变时的信号
    connect(player,&QMediaPlayer::metaDataAvailableChanged,this,&Widget::onMetaDataAvailableChanged);

    //当playlist中播放源发生变化时
    connect(playerList,&QMediaPlaylist::currentIndexChanged,this,&Widget::onCurrentIndexChanged);

    //当播放模式发生改变时
    connect(playerList,&QMediaPlaylist::playbackModeChanged,this,&Widget::onPlayModelClicked);
}
// 初始化数据库
void Widget::initSqlite()
{
    // 1. 进行数据库驱动加载
    sqlite = QSqlDatabase::addDatabase("QSQLITE");

    // 2. 设置数据库名称
    sqlite.setDatabaseName("MiniMusic.db");

    // 3. 打开
    if(!sqlite.open())
    {
        QMessageBox::critical(this,"MiniMusic","数据库打开失败");
        return;
    }
    qDebug() << "MiniMusic数据库连接成功";

    // 4. 创建表
    QString sql = "CREATE TABLE IF NOT EXISTS MusicInfo(\
                  id INTEGER PRIMARY KEY AUTOINCREMENT,\
                  musicId varchar(50) UNIQUE,\
                  musicName varchar(50),\
                  musicSinger varchar(50), \
                  albumName varchar(50),\
                  musicUrl varchar(256),\
                  duration BIGINT,\
                  isLike INTEGER,\
                  isHistory INTEGER)";

    QSqlQuery query;
    if(!query.exec(sql))
    {
        QMessageBox::critical(this,"MiniMusic","初始化错误!!!");
        return;
    }

    qDebug() << "MusicInfo表创建成功!!!";
}

//将数据库中的歌曲初始化到界面
void Widget::initMusicList()
{
    musicList.readFromDB();

    ui->likePage->setMusicListType(PageType::LIKE_PAGE);
    ui->likePage->reFrush(musicList);

    ui->localPage->setMusicListType(PageType::LOCAL_PAGE);
    ui->localPage->reFrush(musicList);

    ui->recentPage->setMusicListType(PageType::HISTORY_PAGE);
    ui->recentPage->reFrush(musicList);
}
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

    //播放所有按钮信号处理(likePage、localPage、recentPage都有playAll按钮)
    connect(ui->likePage,&CommonPage::playAll,this,&Widget::onPlayAll);
    connect(ui->localPage,&CommonPage::playAll,this,&Widget::onPlayAll);
    connect(ui->recentPage,&CommonPage::playAll,this,&Widget::onPlayAll);

    //处理likePage、localPage、recentPage三个页面双击歌曲
    connect(ui->likePage,&CommonPage::playMusicByIndex,this,&Widget::playMusicByIndex);
    connect(ui->localPage,&CommonPage::playMusicByIndex,this,&Widget::playMusicByIndex);
    connect(ui->recentPage,&CommonPage::playMusicByIndex,this,&Widget::playMusicByIndex);

    //静音信号处理
    connect(volumeTool,&VolumeTool::setMusicMuted,this,&Widget::setPlayerMuted);
    //设置音量信号处理
    connect(volumeTool,&VolumeTool::setMusicVolume,this,&Widget::setPlayerVolume);
    //显示歌词
    connect(ui->lrcWord,&QPushButton::clicked,this,&Widget::onLrcWordClicked);

    //musicSLider::setMusicSliderPosition
    connect(ui->progressBar,&MusicSlider::setMusicSliderPosition,this,&Widget::onMusicSliderChanged);
}
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

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

//将Btform动画与currentPage对应显示
void Widget::updateBtformAnimation()
{
    // 获取currentPage在stackedWidget上的索引
    int index = ui->stackedWidget->indexOf(currentPage);
    if(-1 == index)
    {
        qDebug() << "该页面不存在";
        return;
    }

    //获取Widget界面上所有的BtForm
    QList<BtForm*> btForms = this->findChildren<BtForm*>();
    for(auto BtForm : btForms)
    {
        if(BtForm->getPageId() == index)
        {
            BtForm->showAnimal(true);
        }
        else
        {
            BtForm->showAnimal(false);
        }
    }
}

void Widget::onMiniMusicQuit()
{
    // 关闭窗口前将music信息导入数据库
    musicList.writeToDB();

    // 断开与SQLite的连接
    sqlite.close();

    close();
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//窗口按钮模块
//关闭窗口按钮
void Widget::on_quit_clicked()
{
    hide();
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
//换肤
void Widget::on_skin_clicked()
{
    //更换背景颜色 或者 更换背景图片
    QMessageBox::information(this,"温馨提示","换肤功能暂未支持，敬请期待...");
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

    isDrag = false;
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
        isDrag = true;
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
    if(Qt::LeftButton == event->buttons() && isDrag)
    {
        //将窗口移动到相对位置
        move(event->globalPos() - dragPosition);
        return;
    }
    QWidget::mouseMoveEvent(event);
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
// 播放控制区
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
// 播放模式切换
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
// 设置静音
void Widget::setPlayerMuted(bool isMuted)
{
    player->setMuted(isMuted);
}
// 歌词显示
void Widget::onLrcWordClicked()
{
    lrcPage->show();

    lrcPageAnimation->start();
}
// 设置播放器媒体音量
void Widget::setPlayerVolume(int volume)
{
    player->setVolume(volume);
}
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//播放全部歌曲，默认从第0首开始播放
void Widget::onPlayAll(PageType pageType)
{
    ui->play->setIcon(QIcon(":/image/play_2.png"));
    CommonPage* page = ui->localPage;
    switch(pageType)
    {
    case PageType::LIKE_PAGE:
        page = ui->likePage;
        break;
    case PageType::LOCAL_PAGE:
        page = ui->localPage;
        break;
    case PageType::HISTORY_PAGE:
        page = ui->recentPage;
        break;
    default:
        qDebug() << "暂未支持";
    }
    //page中记录的就是要播放的页面
    playAllMusicOfCommonPage(page,0);
}

void Widget::playAllMusicOfCommonPage(CommonPage *page, int index)
{
    currentPage = page;

    updateBtformAnimation();
    //清空之前playlist中的歌曲
    playerList->clear();
    //添加要播放的歌曲
    page->addMusicToPlayList(musicList,playerList);
    //从第n首开始播放
    playerList->setCurrentIndex(index);
    //播放
    player->play();
}
//
void Widget::onCurrentIndexChanged(int index)
{
    currentIndex = index;
    // 由于commonPage中的歌曲和正在播放的歌曲先后次序是相同的
    // 所以知道playlist中的index，就可以到commonPage中获取该歌曲
    QString musicId = currentPage->getMusicIdByIndex(index);

    //通过索引拿到歌曲，修改歌曲的History属性
    auto it = musicList.findMusicById(musicId);
    if(it != musicList.end())
    {
        it->setIsHistory(true);
    }

    ui->recentPage->reFrush(musicList);
}
//更新歌曲总时间
void Widget::onDurationChanged(qint64 duration)
{
    // 将整形总时间转换为min:sec
    // duration/1000/60;
    // duration/1000%60;
    totalTime = duration;

    ui->totalTime->setText(QString("%1:%2").arg(duration/1000/60,2,10,QChar('0'))
                                           .arg(duration/1000%60,2,10,QChar('0')));
}

void Widget::onPositionChanged(qint64 position)
{
    //更新实时播放时间
    ui->currentTime->setText(QString("%1:%2").arg(position/1000/60,2,10,QChar('0'))
                                             .arg(position/1000%60,2,10,QChar('0')));

    //同步进度条位置
    ui->progressBar->setStep(position/(float)totalTime);

    //在歌词界面同步歌词
    if(currentIndex >= 0)
    {
        lrcPage->showLrcWordLine(position);
    }
}

void Widget::onMusicSliderChanged(float ratio)
{
    //根据总宽度与比率的乘积，修改播放时间
    qint64 duration = totalTime * ratio;
    ui->currentTime->setText(QString("%1:%2").arg(duration/1000/60,2,10,QChar('0'))
                                             .arg(duration/1000%60,2,10,QChar('0')));
    //修改事件也要更新媒体元播放的位置
    player->setPosition(duration);
}

void Widget::onMetaDataAvailableChanged(bool available)
{
    (void)available;
    //歌曲名称、歌手直接在music对象中获取
    //需要知道媒体源在播放列表中的索引
    QString musicId = currentPage->getMusicIdByIndex(currentIndex);
    auto it = musicList.findMusicById(musicId);

    QString musicName = "未知歌曲";
    QString musicSinger = "未知歌手";
    if(it != musicList.end())
    {
        musicName = it->getMusicName();
        musicSinger = it->getMusicSinger();
    }
    ui->musicName->setText(musicName);
    ui->musicSinger->setText(musicSinger);

    //通过媒体源获取封面图加载到播放控制区
    QVariant coverImage = player->metaData("ThumbnailImage");
    if(coverImage.isValid())
    {
        QImage image = coverImage.value<QImage>();
        ui->musicCover->setPixmap(QPixmap::fromImage(image));
        currentPage->setMusicImage(QPixmap::fromImage(image));
    }
    else
    {
        qDebug() << "歌曲无封面图";
        QString path = ":/image/pages/030.jpg";
        ui->musicCover->setPixmap(path);
        currentPage->setMusicImage(path);
    }
    ui->musicCover->setScaledContents(true);//图像自动填满容器

    //解析歌曲的LRC歌词
    if(it != musicList.end())
    {
        //获取lrc文件路径
        QString lrcPath = it->getLrcFilePath();

        //解析歌词文件
        lrcPage->parseLrcFile(lrcPath);
    }
}
//通过索引播放歌曲（双击播放）
void Widget::playMusicByIndex(CommonPage *page, int index)
{
    //默认为暂停图标，双击播放要同步播放图标
    ui->play->setIcon(QIcon(":/image/play_2.png"));
    playAllMusicOfCommonPage(page,index);
}


