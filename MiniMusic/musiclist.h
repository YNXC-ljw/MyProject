#ifndef MUSICLIST_H
#define MUSICLIST_H

#include <QVector>
#include <QUrl>
#include <QList>

#include "music.h"

class MusicList
{
public:
    MusicList();

    void addMusicsByUrl(const QList<QUrl>& musicUrls);
private:
    QVector<Music> musicList;
};

#endif // MUSICLIST_H
