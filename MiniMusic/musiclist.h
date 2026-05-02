    #ifndef MUSICLIST_H
#define MUSICLIST_H

#include <QVector>
#include <QUrl>
#include <QList>
#include <QSet>

#include "music.h"

typedef QVector<Music>::iterator Iterator;
class MusicList
{
public:
    MusicList();

    void addMusicsByUrl(const QList<QUrl>& musicUrls);

    Iterator findMusicById(const QString& musicId);

    Iterator begin();
    Iterator end();

    void writeToDB();
    void readFromDB();
private:
    QVector<Music> musicList;

    QSet<QString> musicPaths;   // 防止歌曲文件重复加载
};

#endif // MUSICLIST_H
