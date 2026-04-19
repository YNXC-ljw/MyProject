#include "musiclist.h"
#include <QMimeDatabase>

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
        QMimeType mimeType = mimeDB.mimeTypeForName(e.toLocalFile());
        QString mime = mimeType.name();
        //audio/mepg:mp3                audio/flac:无损音乐
        if(mime == "audio/mepg" || mime == "audio/flac" || mime == "audio/wav")
        {
            //将Urls转换为music对象，再通过musicList进行管理
            Music music(e);
            musicList.push_back(music);
        }
    }
}
