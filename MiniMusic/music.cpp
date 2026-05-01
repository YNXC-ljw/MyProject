#include "music.h"
#include <QUuid>
#include <QMediaPlayer>
#include <QCoreApplication>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

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

void Music::setMusicId(const QString& musicId)
{
    this->musicId = musicId;
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

QString Music::getLrcFilePath() const
{
    QString lrcPath = musicUrl.toLocalFile();
    lrcPath.replace(".mp3",".lrc");
    lrcPath.replace(".wav",".lrc");
    lrcPath.replace(".mpga",".lrc");
    lrcPath.replace(".flac",".lrc");

    return lrcPath;
}

void Music::insertMusicToDB()
{
    // 1. 检查music对象是否存在于数据库中
    QSqlQuery query;
    //query.prepare("select * from MusicInfo where musicId = ?");
    //query.prepare("select id from MusicInfo where musicId = ?");
    query.prepare("select exists (select 1 from MusicInfo where musicId = ?)");//select exists:存在返回true，否则false
    query.addBindValue(musicId);

    if(!query.exec())
    {
        qDebug() << "查询失败:" << query.lastError().text();
        return;
    }

    if(query.next())
    {
        bool isExists = query.value(0).toBool();

        if(isExists)
        {
            //歌曲存在
            // 2. 存在：不需要插入；此时只需要将isLike和isHistory属性更新即可
            query.prepare("UPDATE MusicInfo SET isLike = ?, isHistory = ? WHERE musicId = ?");
            query.addBindValue(isLike ? 1 : 0);
            query.addBindValue(isHistory ? 1 : 0);
            query.addBindValue(musicId);
            if (!query.exec())
            {
                qDebug() << "更新失败：" << query.lastError().text();
            }

            qDebug() << "更新music信息：" << musicName << " " << musicId;
        }
        else
        {
            // 3. 不存在：直接将music对象属性插入数据库
            query.prepare("INSERT INTO MusicInfo(musicId, musicName, musicSinger, albumName\
                                                  , musicUrl, duration, isLike, isHistory) \
                          VALUES(?, ?, ?, ?, ?, ?, ?, ?)");
            query.addBindValue(musicId);
            query.addBindValue(musicName);
            query.addBindValue(musicSinger);
            query.addBindValue(musicAlbum);
            query.addBindValue(musicUrl.toLocalFile());
            query.addBindValue(duration);
            query.addBindValue(isLike ? 1 : 0);
            query.addBindValue(isHistory ? 1 : 0);

            if (!query.exec())
            {
                qDebug() << "插入失败：" << query.lastError().text();
                return;
            }

            qDebug() << "插入music信息：" << musicName << " " << musicId;
        }

    }

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
        QCoreApplication::processEvents();
    }
    //到此说明已经加载完成，可以获取到有效的元数据
    if(player.isMetaDataAvailable())
    {
        musicName = player.metaData("Title").toString();
        musicSinger = player.metaData("Auther").toString();
        musicAlbum = player.metaData("AlbumTitle").toString();
        duration = player.metaData("Duration").toLongLong();

        ////////////////////////////////////////////////////////////////////////////////////////////////
        //如果上述方法遇到格式不对或者盗版音乐，获取不到歌曲信息
        //那就通过获取文件名的方式自己获取并打印对应的信息
        QString filename = musicUrl.fileName();

        //"马也_Crabbit+-+海屿你.mp3"
        //"海屿你.mp3"
        int index = filename.indexOf("-");
        if(musicName.isEmpty())
        {
            if(index != -1)
            {
                musicName = filename.mid(index+1,filename.indexOf(".") - index - 1).trimmed();
            }
            else
            {
                musicName = "未知音乐";
            }
        }
        //如果获取到的musicSinger为" "，就自己处理
        if(musicSinger.isEmpty())
        {
            if(index != -1)
            {
                musicSinger = filename.mid(0, index);
            }
            else
            {
                musicSinger = "位置歌手";
            }
        }
        if(musicAlbum.isEmpty())
        {
            musicAlbum = "未知专辑";
        }
        qDebug() << filename;
        qDebug() << musicName << ":" << musicSinger << ":" << musicAlbum << ":" << duration;
    }


}
