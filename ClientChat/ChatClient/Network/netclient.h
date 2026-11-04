#ifndef NETCLIENT_H
#define NETCLIENT_H

#include <QNetworkAccessManager>
#include <QWebSocket>
#include <QObject>
#include <QProtobufSerializer>
// 此处为了避免循环包含头文件，就需要前置声明来解决问题
namespace model{
    class DataCenter;
}

namespace network{

class NetClient : public QObject
{
    Q_OBJECT

private:
    // 定义重要常量,ip 都定义为本地换回 ip，端口号约定为8000和8001(一个进程可以绑定多个端口号)
    const QString HTTP_URL = "http://127.0.0.1:8080";
    const QString WEBSOCKET_URL = "ws://127.0.0.1:8001/ws";

public:
    NetClient(model::DataCenter* dataCenter);

    void ping();
private:
    model::DataCenter* dataCenter;

    // HTTP 客户端
    QNetworkAccessManager httpClient;
    // WebSocket 客户端
    QWebSocket webSocketClient;

    // 序列化器
    QProtobufSerializer serializer;
signals:
};

} // end namespace
#endif // NETCLIENT_H
