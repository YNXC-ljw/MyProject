#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "volumetool.h"
#include "musiclist.h"


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
    //窗口按钮区设置图片
    void settingBox();
    //播放控制区设置图片
    void contralMusic();
    //管理所有信号与信号槽的函数
    void connectSignalAndSlots();

    QJsonArray randomPiction();
private slots:
    //关闭窗口
    void on_quit_clicked();
    //窗口最小化
    void on_min_clicked();
    //窗口最大化
    void on_max_clicked();

    void onBtClicked(int pageId);



    void on_volume_clicked();

    void on_addLocal_clicked();

protected:
    //重写鼠标点击函数
    void mousePressEvent(QMouseEvent *event);
    //重写鼠标移动函数
    void mouseMoveEvent(QMouseEvent *event);

private:
    Ui::Widget *ui;

    QPoint dragPosition;

    VolumeTool* volumeTool;

    MusicList musicList;
};
#endif // WIDGET_H
