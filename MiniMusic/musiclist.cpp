#include "musiclist.h"
#include <QMimeDatabase>
#include <QDebug>
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
