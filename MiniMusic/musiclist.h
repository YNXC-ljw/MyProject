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

    // 将工作线程解析好的歌曲去重后放入列表（不再进行元数据解析）
    void addParsedMusics(const QList<Music> &musics);

    Iterator findMusicById(const QString& musicId);

    Iterator begin();
    Iterator end();

    // 数据库读写
    void writeToDB();
    void readFromDB();
private:
    QVector<Music> musicList; // 存放所有音乐

    QSet<QString> musicPaths;   // 防止歌曲文件重复加载
};

#endif // MUSICLIST_H
