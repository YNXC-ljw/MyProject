#pragma once

#include <QString>
#include <QIcon>
#include <QUuid>
#include <QFile>
#include <QDebug>
#include <QDateTime>
#include <QFileInfo>

namespace model
{
//////////////////////////////////////////////////
/// 工具函数，后续很多模块可能用到
//////////////////////////////////////////////////

static inline QString getFileName(const QString& path) {
    QFileInfo fileInfo(path);
    return fileInfo.fileName();
}

// 封装一个 "宏" 作为打印日志的方式.
#define TAG QString("[%1:%2]").arg(model::getFileName(__FILE__), QString::number(__LINE__))
// #define TAG "[" << __LINE__ << "]"

// qDebug 打印字符串的时候，就会自动加上 " "
#define LOG() qDebug().noquote() << TAG


// 函数定义如果写在.h文件中，必须加static或inline或全都加，避免链接时出现“函数重定义”错误
static inline QString formatTime(int64_t timestamp)
{
    // 时间戳转换为QDateTime对象
    QDateTime datetime = QDateTime::fromSecsSinceEpoch(timestamp);
    // 将datetime对象转换为“格式化时间”
    return datetime.toString("MM-dd HH:mm:ss");
}

// 通过时间戳获取秒级时间
static inline int64_t getTime()
{
    return QDateTime::currentSecsSinceEpoch();
}

// 根据QByteArray转换成QIcon方便网络上传头像
static inline QIcon makeIcon(const QByteArray& byteArray)
{
    QPixmap pixmap;
    pixmap.loadFromData(byteArray);
    QIcon icon(pixmap);
    return icon;
}

// 读写文件（从指定文件中读取所有二进制内容，得到一个QByteArray 或 反过来写）
static inline void writeFileFromQByteArray(const QString& path,const QByteArray& content)
{
    QFile file(path);
    bool ok = file.open(QFile::WriteOnly);
    if(!ok)
    {
        LOG() << "打开文件失败!";
        return;
    }
    file.write(content);
    file.flush(); // 刷新缓冲区
    file.close();
}
static inline QByteArray readFiletoQByteArray(const QString& path)
{
    QFile file(path);
    bool ok = file.open(QFile::ReadOnly);
    if(!ok)
    {
        LOG() << "文件打开失败!";
        return QByteArray();
    }
    QByteArray content = file.readAll();
    file.close();
    return content;
}
//////////////////////////////////////////////////
/// 用户信息
//////////////////////////////////////////////////

class UserInfo
{
public:
    QString userId = "";              // 用户编号
    QString nickName = "";            // 用户昵称
    QString description = "";         // 用户签名
    QString phone = "";               // 手机号码
    QIcon avatar;                     // 用户头像
};
//////////////////////////////////////////////////
/// 消息信息
//////////////////////////////////////////////////
enum MessageType
{
    TEXT_TYPE,          // 文本消息
    IMAGE_TYPE,         // 图片消息
    FILE_TYPE,          // 文件消息
    SPEECH_TYPE         // 语音消息
};

class Message
{
public:
    QString messageId = "";                  // 消息编号
    QString chatSessionId = "";             // 消息所属会话的编号
    QString time = "";                      // 消息时间，通过格式化时间的方式来表示，形如 08-06 19:00:01
    MessageType messageType = TEXT_TYPE;    // 消息类型
    UserInfo sender;            // 消息发送者
    QByteArray content;         // 消息内容(QT表示二进制文本必须使用QByteArray,字符才能用QString)
    QString fileId = "";             // 文件的身份标识，为图片、文件、语音时生效；为普通文本时为""
    QString fileName = "";           // 文件名称，为文件时生效，其余为空

    // extraInfo 只是当消息为文件消息时，作为文件名补充
    static Message makeMessage(MessageType messageType,const QString& chatSessionId,const UserInfo& sender,
        const QByteArray& content,const QString& extraInfo)
    {
        if(messageType == TEXT_TYPE)
        {
            return makeTextMessage(chatSessionId,sender,content);
        }
        else if(messageType == IMAGE_TYPE)
        {
            return makeImageMessage(chatSessionId,sender,content);
        }
        else if(messageType == FILE_TYPE)
        {
            return makeFileMessage(chatSessionId,sender,content,extraInfo);
        }
        else if(messageType == SPEECH_TYPE)
        {
            return makeSpeechMessage(chatSessionId,sender,content);
        }
        else{
            return Message();
        }
    }

    // 通过UUid生成具有全球唯一的id值
    static QString makeId()
    {
        return QUuid::createUuid().toString().sliced(25,12);
    }
private:
    static Message makeTextMessage(const QString& chatSessionId, const UserInfo& sender, const QByteArray& content) {
        Message message;

        message.messageId = makeId();
        message.chatSessionId = chatSessionId;
        message.sender = sender;
        message.content = content;
        message.time = formatTime(getTime());
        message.messageType = TEXT_TYPE;
        // 对于文本消息这俩为空
        message.fileId = "";
        message.fileName = "";

        return message;
    }

    static Message makeImageMessage(const QString& chatSessionId, const UserInfo& sender, const QByteArray& content) {
        Message message;

        message.messageId = makeId();
        message.chatSessionId = chatSessionId;
        message.sender = sender;
        message.content = content;
        message.time = formatTime(getTime());
        message.messageType = IMAGE_TYPE;
        // fileId暂时为空，filename为空
        message.fileId = "";
        message.fileName = "";

        return message;
    }

    static Message makeFileMessage(const QString& chatSessionId, const UserInfo& sender, const QByteArray& content, const QString &extraInfo){
        Message message;

        message.messageId = makeId();
        message.chatSessionId = chatSessionId;
        message.sender = sender;
        message.content = content;
        message.time = formatTime(getTime());
        message.messageType = FILE_TYPE;

        message.fileId = "";
        message.fileName = extraInfo;

        return message;
    }

    static Message makeSpeechMessage(const QString& chatSessionId, const UserInfo& sender, const QByteArray& content) {
        Message message;

        message.messageId = makeId();
        message.chatSessionId = chatSessionId;
        message.sender = sender;
        message.content = content;
        message.time = formatTime(getTime());
        message.messageType = SPEECH_TYPE;
        // 对于文本消息这俩为空
        message.fileId = "";
        message.fileName = "";

        return message;
    }
};

//////////////////////////////////////////////////
/// 会话信息
//////////////////////////////////////////////////

class chatSessionInfo
{
public:
    QString chatSessionId = "";          // 会话编号
    QString chatSessionName = "";        // 会话名称(单聊即对方名称，群聊则群名)
    Message lastMessage;                 // 会话最后一条消息
    QIcon avatar;                        // 会话头像
    QString userId = "";                 // 会话
};
}        // end model