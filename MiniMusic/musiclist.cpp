#include "musiclist.h"
#include <QMimeDatabase>
#include <QDebug>
#include <QSqlQuery>
MusicList::MusicList()
{

}

void MusicList::addMusicsByUrl(const QList<QUrl> &musicUrls)
{
    for(auto e : musicUrls)
    {
        //将收到的musicUrls文件通过检测，确认其为歌曲文件再将其添加到列表中
        //检测歌曲文件的MIME类型
        QMimeDatabase mimeDB;
        QMimeType mimeType = mimeDB.mimeTypeForFile(e.toLocalFile());
        QString mime = mimeType.name();

        //audio/mpeg:mp3                audio/flac:无损音乐            .wav文件的标准为audio/wav;常见为audio/x-wav
        if(mime == "audio/mpeg" || mime == "audio/flac" || mime == "audio/x-wav")
        {
//            qDebug() << e << ":" << mime;
            //将Urls转换为music对象，再通过musicList进行管理
            Music music(e);
            musicList.push_back(music);
        }
    }
}

Iterator MusicList::findMusicById(const QString& musicId)
{
    for(auto it = begin();it != end();it++)
    {
        if(it->getMusicId() == musicId)
        {
            return it;
        }
    }
    return end();
}

Iterator MusicList::begin()
{
    return musicList.begin();
}

Iterator MusicList::end()
{
    return musicList.end();
}

void MusicList::writeToDB()
{
    for(auto music : musicList)
    {
        // 将一个个music对象的所有属性写入到数据库中
        music.insertMusicToDB();
    }
}

void MusicList::readFromDB()
{
    QSqlQuery query;
    query.prepare("select musicId,musicName,musicSinger,albumName,musicUrl\
                 ,duration,isLike,isHistory from MusicInfo");
    if(!query.exec())
    {
        qDebug() << "数据库查询失败";
        return;
    }

    while(query.next())
    {
         Music music;
         music.setMusicId(query.value(0).toString());
         music.setMusicName(query.value(1).toString());
         music.setMusicSinger(query.value(2).toString());
         music.setMusicAlbum(query.value(3).toString());
         music.setMusicUrl(query.value(4).toString());
         music.setDuration(query.value(5).toLongLong());
         music.setIsLike(query.value(6).toBool());
         music.setIsHistory(query.value(7).toBool());
         musicList.push_back(music);

    }
}







