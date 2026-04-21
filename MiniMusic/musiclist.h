    #ifndef MUSICLIST_H
#define MUSICLIST_H

#include <QVector>
#include <QUrl>
#include <QList>

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
private:
    QVector<Music> musicList;
};

#endif // MUSICLIST_H
