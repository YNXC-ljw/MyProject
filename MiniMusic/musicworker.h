#ifndef MUSICWORKER_H
#define MUSICWORKER_H

#include <QObject>
#include <QList>
#include <QUrl>

#include "music.h"

class MusicWorker : public QObject
{
    Q_OBJECT
public:
    explicit MusicWorker(QObject *parent = nullptr);

public slots:
    // 解析歌曲：在工作线程中把 URL 转换成 Music 对象（含元数据解析）
    void parseMusics(const QList<QUrl> &urls);

signals:
    // 解析完成后，一次性把结果传回主线程
    void musicsParsed(const QList<Music> &musics);
    // 通知主线程本次任务已经执行完成
    void workFinished();
};

#endif // MUSICWORKER_H
