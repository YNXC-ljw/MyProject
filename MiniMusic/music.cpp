#include "music.h"
#include <QUuid>
#include <QMediaPlayer>
Music::Music()
    :isLike(false)
    ,isHistory(false)
{

}

Music::Music(QUrl url)
    :isLike(false)
    ,isHistory(false)
    ,musicUrl(url)
{
    //读取url中对应的歌曲信息，获取元数据
    //歌曲名称、作者、专辑......
    musicId = QUuid::createUuid().toString();
    parseMediaMetaMusic();
}
void Music::setMusicName(const QString& musicName)
{
    this->musicName = musicName;
}
void Music::setMusicSinger(const QString &musicSinger)
{
    this->musicSinger = musicSinger;
}
void Music::setMusicAlbum(const QString& musicAlbum)
{
    this->musicAlbum = musicAlbum;
}
void Music::setDuration(qint64 duration)
{
    this->duration = duration;
}
void Music::setIsLike(bool isLike)
{
    this->isLike = isLike;
}
void Music::setIsHistory(bool isHistory)
{
    this->isHistory = isHistory;
}
void Music::setMusicUrl(QUrl musicUrl)
{
    this->musicUrl = musicUrl;
}
///////////////////////////////////////////////////////////////////
QString Music::getMusicName()const
{
    return musicName;
}
QString Music::getMusicSinger()const
{
    return musicSinger;
}
QString Music::getMusicAlbum()const
{
    return musicAlbum;
}
qint64 Music::getDuration()const
{
    return duration;
}
bool Music::getIsLike()const
{
    return isLike;
}
bool Music::getIsHistory()const
{
    return isHistory;
}
QUrl Music::getMusicUrl()const
{
    return musicUrl;
}

QString Music::getMusicId() const
{
    return musicId;
}

void Music::parseMediaMetaMusic()
{
    // 1. 创建媒体播放对象
    QMediaPlayer player;

    // 2. 设置媒体元，依靠setMedia解析元数据
    player.setMedia(musicUrl);

    // 3. 因为setMedia不会等待音乐的加载，立马返回
    //    因此在通过mediaMeta方法获取元数据时，需要保证   媒体已经加载完毕
    //    可以通过isMetaDataAvailabel检测数据是否有效
    //    如果该方法返回true则说明加载完成

    while(!player.isMetaDataAvailable())
    {
        ;
    }
    if(player.isMetaDataAvailable())
    {
        musicName = player.metaData("Title").toString();
        musicSinger = player.metaData("Auther").toString();
        musicAlbum = player.metaData("AlbumTitle").toString();
        duration = player.metaData("Duration").toLongLong();
    }

}
