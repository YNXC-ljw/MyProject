#ifndef LISTITEM_H
#define LISTITEM_H

#include <QWidget>

namespace Ui {
class ListItem;
}

class ListItem : public QWidget
{
    Q_OBJECT

public:
    explicit ListItem(QWidget *parent = nullptr);
    ~ListItem();

    //QWidget不支持hover，要想达到该效果必须重写函数进行拦截
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

    void setMusicName(const QString& musicName);
    void setMusicSinger(const QString& musicSinger);
    void setMusicAlbum(const QString& musicAlbum);

    void setLikeMusic(bool isLike);
    void onLikeBtnClicked();

signals:
    void setIsLike(bool isLike);
private:
    Ui::ListItem *ui;

    bool isLike;
};

#endif // LISTITEM_H
