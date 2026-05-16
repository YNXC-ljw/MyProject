#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QPropertyAnimation>
#include <QSqlDatabase>

#include "volumetool.h"
#include "musiclist.h"
#include "commonpage.h"
#include "lrcpage.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void initUi();
    void playerInit();
    void initSqlite();
    void initMusicList();
    //窗口按钮区设置图片
    void settingBox();
    //播放控制区设置图片
    void contralMusic();
    //管理所有信号与信号槽的函数
    void connectSignalAndSlots();

    QJsonArray randomPiction();

    void updateBtformAnimation();

    void onMiniMusicQuit();
private slots:
    //关闭窗口
    void on_quit_clicked();
    //窗口最小化
    void on_min_clicked();
    //窗口最大化
    void on_max_clicked();

    void onBtClicked(int pageId);
    void updateLikeMusicAndPage(bool isLike,const QString& musicId);


    void on_volume_clicked();

    void on_addLocal_clicked();
////////////////////////////////////////////////////////////////////////////
    //播放控制区的槽函数
    void onPlayMiusic();
    void onPlayUpClicked();//上一首
    void onPlayDownClicked();//下一首
    void onPlayModelClicked();//播放模式
    void setPlayerMuted(bool isMuted);//静音
    void onLrcWordClicked();    //歌词显示
///////////////////////////////////////////////////////////////////////////
    //volumeTool类中setMusicVolume信号对应的槽函数
    void setPlayerVolume(int volume);

    //播放所有按钮槽函数
    void onPlayAll(PageType pageType);
    void playAllMusicOfCommonPage(CommonPage *page,int index);
//////////////////////////////////////////////////////////////////////////////
    //QMeidaPlayer中stateChanged信号对应槽函数
    void onCurrentIndexChanged(int);//播放媒体切换
    void onDurationChanged(qint64 duration);
    void onPositionChanged(qint64 position);//播放进度发生改变
    void onMusicSliderChanged(float ratio);//进度条发生改变时修改音乐播放时间
    void onMetaDataAvailableChanged(bool available);//元数据发生改变时

    //双击播放槽函数
    void playMusicByIndex(CommonPage* page,int index);



    void on_skin_clicked();//换肤

protected:
    //重写鼠标点击函数
    void mousePressEvent(QMouseEvent *event);
    //重写鼠标移动函数
    void mouseMoveEvent(QMouseEvent *event);

private:
    Ui::Widget *ui;

    QPoint dragPosition;

    VolumeTool* volumeTool;
    LrcPage *lrcPage;
    QPropertyAnimation* lrcPageAnimation;

    MusicList musicList;    //组织歌曲文件

    QMediaPlayer* player;   //专门用于播放控制
    QMediaPlaylist* playerList; //专门用来管理播放源，包含播放设置

    int currentIndex;       //记录当前播放元在列表中的索引

    CommonPage* currentPage; //记录当前播放歌曲的页面
    qint64 totalTime;       //记录媒体元的总时间

    QSqlDatabase sqlite;

    bool isDrag;    //是否拖拽
};
#endif // WIDGET_H
