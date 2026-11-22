#include "model/data.h"
#include "soundrecorder.h"

#include <QMediaDevices>
#include <QDir>

using namespace model;

//////////////////////////////////////////////////////////
/// 单例模式
//////////////////////////////////////////////////////////
SoundRecorder* SoundRecorder::instance = nullptr;

SoundRecorder *SoundRecorder::getInstance()
{
    if(instance == nullptr)
    {
        instance = new SoundRecorder();
    }
    return instance;
}

void SoundRecorder::startRecord()
{
    auto result = soundFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
    (void) result;
    audioSource->start(&soundFile);
}

void SoundRecorder::stopRecord()
{
    audioSource->stop();
    soundFile.close();
    // 通知调用者，录制结束做一些相应的状态切换
    emit this->soundRecordDone(RECORD_PATH);
}

void SoundRecorder::startPlay(const QByteArray& content)
{
    if (content.isEmpty()) {
        return;
    }
    // 1. 把数据写入到临时文件
    model::writeFileFromQByteArray(PLAY_PATH, content);

    // 2. 播放语音
    inputFile.setFileName(PLAY_PATH);
    inputFile.open(QIODevice::ReadOnly);
    audioSink->start(&inputFile);

}

void SoundRecorder::stopPlay()
{
    audioSink->stop();
    inputFile.close();
}

SoundRecorder::SoundRecorder(QObject *parent)
    : QObject{parent}
{
    // 1.创建存放音频对应目录
    QDir soundRootPath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    soundRootPath.mkdir("sound");

    // 2.设置路径名并打开文件
    soundFile.setFileName(RECORD_PATH);

    // 3.配置录制的格式
    QAudioFormat format;
    format.setSampleRate(160000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    // 4.配置录制音频的设备
    QAudioDevice device = QMediaDevices::defaultAudioInput();
    if(!device.isFormatSupported(format))
    {
        LOG() << "该设备不支持所设置的录制格式!";
        return;
    }

    // 5.设置完毕，创建录制对象
    audioSource = new QAudioSource(format,this);
    connect(audioSource,&QAudioSource::stateChanged,this,[=](QAudio::State state){
        if(QtAudio::StoppedState == state)
        {
            // 录制完毕
            if(audioSource->error() != QAudio::NoError)
            {
                LOG() << audioSource->error();
            }
        }
    });

    // 6. 初始化播放模块
    outputDevices = new QMediaDevices(this);
    outputDevice = outputDevices->defaultAudioOutput();
    QAudioFormat outputFormat;
    outputFormat.setSampleRate(16000);
    outputFormat.setChannelCount(1);
    outputFormat.setSampleFormat(QAudioFormat::Int16);
    if (!outputDevice.isFormatSupported(outputFormat)) {
        LOG() << "播放设备，格式不支持";
        return;
    }
    audioSink = new QAudioSink(outputDevice, outputFormat);

    connect(audioSink, &QAudioSink::stateChanged, this, [=](QtAudio::State state) {
        if (state == QtAudio::IdleState) {
            LOG() << "IdleState";
            this->stopPlay();
            emit this->soundPlayDone();
        } else if (state == QAudio::ActiveState) {
            LOG() << "ActiveState";
        } else if (state == QAudio::StoppedState) {
            LOG() << "StoppedState";
            if (audioSink->error() != QtAudio::NoError) {
                LOG() << audioSink->error();
            }
        }
    });

}













