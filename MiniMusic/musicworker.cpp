#include "musicworker.h"
#include <QMimeDatabase>
#include <QMimeType>
#include <QDebug>

MusicWorker::MusicWorker(QObject *parent) : QObject(parent)
{

}

void MusicWorker::parseMusics(const QList<QUrl> &urls)
{
    QList<Music> result;
    QMimeDatabase mimeDB;

    for(const QUrl &url : urls)
    {
        QString path = url.toLocalFile();

        // 过滤不支持的音频格式
        QString mime = mimeDB.mimeTypeForFile(path).name();

        if(mime != "audio/mpeg"
            && mime != "audio/flac"
            && mime != "audio/x-wav"
            && mime != "audio/wav")
        {
            continue;
        }

        // Music 构造函数会解析元数据
        // 现在这个操作发生在工作线程，不会阻塞主线程
        Music music(url);
        result.append(music);
    }

    // 把解析好的结果发送回主线程
    emit musicsParsed(result);
    emit workFinished();
}
