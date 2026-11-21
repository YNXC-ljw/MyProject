#ifndef SOUNDRECORDER_H
#define SOUNDRECORDER_H

#include <QObject>
#include <QStandardPaths>
#include <QFile>
#include <QAudioSource>

class SoundRecorder : public QObject
{
    Q_OBJECT
public:
    const QString RECORD_PATH = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "sound/tmpRecord.pcm";
    const QString PLAY_PATH = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "sound/tmpPlay.pcm";
public:
    ~SoundRecorder();
    static SoundRecorder* getInstance();

////////////////////////////////////////
/// 录制
////////////////////////////////////////
    // 开始录制
    void startRecord();
    // 停止录制
    void stopRecord();
////////////////////////////////////////
/// 播放
////////////////////////////////////////
    void startPlay();
    void stopPlay();
private:
    static SoundRecorder* instance;
    explicit SoundRecorder(QObject *parent = nullptr);

    QFile soundFile; // 录制成型的音频文件
    QAudioSource* audioSource;
signals:
    void soundRecordDone(const QString& path);
    void soundPlayDone();
};

#endif // SOUNDRECORDER_H
