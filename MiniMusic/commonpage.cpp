#include "commonpage.h"
#include "ui_commonpage.h"
#include "listitem.h"

#include <QDebug>

CommonPage::CommonPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommonPage)
{
    ui->setupUi(this);

    ui->pageMusicList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(ui->playAllButton,&QPushButton::clicked,this,[=](){
       //点击播放所有按钮之后，通知Widget播放当前pageType页面所有被标记歌曲
        emit playAll(pageType);
    });

    connect(ui->pageMusicList,&QListWidget::doubleClicked,this,[=](const QModelIndex& index){
        emit playMusicByIndex(this,index.row());
    });
}

CommonPage::~CommonPage()
{
    delete ui;
}

void CommonPage::setMusicListType(PageType pageType)
{
    this->pageType = pageType;
}

void CommonPage::setCommonPageUi(const QString& text,const QString& imagePath)
{
    ui->pageTitle->setText(text);
    ui->musicImageLabel->setPixmap(QPixmap(imagePath));
    ui->musicImageLabel->setScaledContents(true);


}

void CommonPage::addMusicToMusicPage(MusicList &musicList)
{
    //每调用一次该函数都会重复添加原先已经存在过的歌曲，所以要新增歌曲就要把原先已经存在的清除掉
    musicOfPage.clear();

    for(auto music : musicList)//直接使用范围for遍历不可取，因为自定义类MusicList未支持迭代器
    {
        switch(pageType)
        {
            case LIKE_PAGE:
                if(music.getIsLike())
                {
                    musicOfPage.push_back(music.getMusicId());
                }
                break;
            case LOCAL_PAGE:
                musicOfPage.push_back(music.getMusicId());
                break;
            case HISTORY_PAGE:
                if(music.getIsHistory())
                {
                    musicOfPage.push_back(music.getMusicId());
                }
                break;
            default:
                qDebug()<<"暂未支持";
        }
    }
}
//将一个个歌曲listitem信息刷新到对应的page页面
void CommonPage::reFrush(MusicList& musicList)
{
    //将QWidgetList之前添加的内容全部清空
    ui->pageMusicList->clear();

    //添加新的歌曲
    addMusicToMusicPage(musicList);
    for(auto musicId : musicOfPage)//有了id就该获取到此id对应的所有数据
    {
        auto it = musicList.findMusicById(musicId);//通过id查找对应的文件
        if(it == musicList.end())
            continue;

        //将Music中的歌曲名称、作者、专辑名称更新到界面中
        ListItem* listItem = new ListItem(this);

        listItem->setMusicName(it->getMusicName());
        listItem->setMusicSinger(it->getMusicSinger());
        listItem->setMusicAlbum(it->getMusicAlbum());
        listItem->setLikeMusic(it->getIsLike());

        QListWidgetItem* item = new QListWidgetItem(ui->pageMusicList);
        item->setSizeHint(QSize(listItem->width(),listItem->height()));
        ui->pageMusicList->setItemWidget(item,listItem);

        //将传递过来的信号进行拦截，自己处理
        connect(listItem,&ListItem::setIsLike,this,[=](bool isLike){
            //更新歌曲状态
            //通知widget更新likePage、localPage、recentPage中的歌曲信息
            it->setIsLike(isLike);
            emit updateLikeMusic(isLike,it->getMusicId());
        });
    }
    //触发窗口重绘事件:paintEvent
    //update();     //不会立马处理paintEvent，而是将它放到消息处理队列中
    repaint();      //立马处理paintEvent
}

// 将当前页面存在的歌曲一个个放进播放队列当中
// musicList里是所有歌曲，但是如果当前页面是"我喜欢"页面，那么只加载是"我喜欢"的歌曲到播放列表中
void CommonPage::addMusicToPlayList(MusicList &musicList, QMediaPlaylist *playList)
{
    for(auto music : musicList)
    {
        switch(pageType)
        {
        case LIKE_PAGE:
            if(music.getIsLike())
            {
                playList->addMedia(music.getMusicUrl());
            }
            break;
        case LOCAL_PAGE:
            playList->addMedia(music.getMusicUrl());
            break;
        case HISTORY_PAGE:
            if(music.getIsHistory())
            {
                playList->addMedia(music.getMusicUrl());
            }
            break;
        default:
            qDebug() << "未支持页面";
        }
    }
}

void CommonPage::setMusicImage(QPixmap pixmap)
{
    ui->musicImageLabel->setPixmap(pixmap);
    ui->musicImageLabel->setScaledContents(true);
}

QString CommonPage::getMusicIdByIndex(int index)
{
    if(index >= musicOfPage.size())
    {
        qDebug() << "没有此歌曲";
        return " ";
    }
    return musicOfPage[index];
}


