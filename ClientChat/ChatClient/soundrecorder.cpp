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

void SoundRecorder::startPlay()
{

}

void SoundRecorder::stopPlay()
{

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
}













