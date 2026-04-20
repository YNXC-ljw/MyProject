#ifndef COMMONPAGE_H
#define COMMONPAGE_H

#include <QWidget>
#include "musiclist.h"

namespace Ui {
class CommonPage;
}

enum PageType
{
    LIKE_PAGE,      //我喜欢
    LOCAL_PAGE,     //本地下载
    HISTORY_PAGE    //历史播放
};
class CommonPage : public QWidget
{
    Q_OBJECT

public:
    explicit CommonPage(QWidget *parent = nullptr);
    ~CommonPage();

    void setMusicListType(PageType pageType);

    void setCommonPageUi(const QString& text,const QString& imagePath);

    void addMusicToMusicPage(MusicList& musicList);

    void reFrush(MusicList& musicList);


private:
    Ui::CommonPage *ui;

    PageType pageType;
    QVector<QString> musicOfPage; //保存PageType对应页面的歌曲ID
};

#endif // COMMONPAGE_H
