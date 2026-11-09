#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QHttpServer>
#include <QWebSocketServer>
#include <QProtobufSerializer>
#include <QFile>
#include <QFileDialog>

//////////////////////////////////////////////////
/// 工具函数，后续很多模块可能用到
//////////////////////////////////////////////////

static inline QString getFileName(const QString& path) {
    QFileInfo fileInfo(path);
    return fileInfo.fileName();
}

// 封装一个 "宏" 作为打印日志的方式.
#define TAG QString("[%1:%2]").arg(getFileName(__FILE__), QString::number(__LINE__))
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

///////////////////////////////////////////////////////////////
/// HTTP 服务器
///////////////////////////////////////////////////////////////
class HttpServer : public QObject
{
    Q_OBJECT
public:
    static HttpServer* getInstance();

    // 通过这个函数，针对 HTTP Server 进行初始化(绑定端口、配置路由......)
    bool init();

    // 获取个人信息
    QHttpServerResponse getUserInfo(const QHttpServerRequest& req);
    // 获取好友列表
    QHttpServerResponse getFriendList(const QHttpServerRequest& req);
    // 获取会话列表
    QHttpServerResponse getChatSessionList(const QHttpServerRequest& req);
    // 获取好友申请列表
    QHttpServerResponse getApplyList(const QHttpServerRequest& req);
private:
    static HttpServer* instance;
    HttpServer() {};
    // HTTP Server
    QHttpServer httpServer;
    // 序列化器
    QProtobufSerializer serializer;
};

///////////////////////////////////////////////////////////////
/// Websocket 服务器
///////////////////////////////////////////////////////////////
class WebSocketServer : public QObject
{
    Q_OBJECT
public:
    static WebSocketServer* getInstance();

    // 针对webSocket服务器进行初始化操作
    bool init();
private:
    static WebSocketServer* instance;
    WebSocketServer() : webSocketServer("websocket server",QWebSocketServer::NonSecureMode){};
    // WebSocket 服务器
    QWebSocketServer webSocketServer;
    // 序列化器
    QProtobufSerializer serializer;
};

#endif // SERVER_H
