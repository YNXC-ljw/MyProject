#ifndef MUSIC_H
#define MUSIC_H

#include <QUrl>

class Music
{
public:
    Music();
    Music(QUrl url);

    void setMusicId(const QString& musicId);
    void setMusicName(const QString& musicName);
    void setMusicSinger(const QString& musicSinger);
    void setMusicAlbum(const QString& musicAlbum);
    void setDuration(qint64 duration);
    void setIsLike(bool isLike);
    void setIsHistory(bool isHistory);
    void setMusicUrl(QUrl musicUrl);

    QString getMusicName()const;
    QString getMusicSinger()const;
    QString getMusicAlbum()const;
    qint64 getDuration()const;
    bool getIsLike()const;
    bool getIsHistory()const;
    QUrl getMusicUrl()const;
    QString getMusicId()const;

    QString getLrcFilePath()const;

    //将歌曲属性写入到数据库
    void insertMusicToDB();

private:
    void parseMediaMetaMusic();
private:
    //给歌曲设置id标识唯一性
    QString musicId;
    //歌曲名称
    QString musicName;
    //歌曲作者
    QString musicSinger;
    //歌曲专辑
    QString musicAlbum;
    //持续时长
    qint64 duration;
    //是否收藏
    bool isLike;
    //是否为历史播放
    bool isHistory;
    //音乐路径
    QUrl musicUrl;
};

#endif // MUSIC_H
