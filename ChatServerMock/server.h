#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QHttpServer>
#include <QProtobufSerializer>

///////////////////////////////////////////////////////////////
/// HTTP 服务器
///////////////////////////////////////////////////////////////
class HttpServer : public QObject
{
    Q_OBJECT
public:
    static HttpServer* getInstance();

    void init();
private:
    static HttpServer* instance;
    // HTTP Server
    QHttpServer httpServer;
    // 序列化器
    QProtobufSerializer serializer;
};

///////////////////////////////////////////////////////////////
/// Websocket 服务器
///////////////////////////////////////////////////////////////

#endif // SERVER_H
