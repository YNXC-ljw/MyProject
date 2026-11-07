#include "server.h"
#include "base.qpb.h"
#include "gateway.qpb.h"
#include "user.qpb.h"
#include "friend.qpb.h"
#include "file.qpb.h"
#include "notify.qpb.h"
#include "speech.qpb.h"
#include "message.qpb.h"
#include "transmite.qpb.h"
#include <QTcpServer>

///////////////////////////////////////////////////////////////
/// HTTP 服务器
///////////////////////////////////////////////////////////////

HttpServer* HttpServer::instance = nullptr;

HttpServer *HttpServer::getInstance()
{
    if(instance == nullptr)
    {
        instance = new HttpServer();
    }
    return instance;
}

bool HttpServer::init()
{
    auto *tcpServer = new QTcpServer(this);

    if(!tcpServer->listen(QHostAddress::Any,8080) || !httpServer.bind(tcpServer))
    {
        qWarning() << "无法启动HTTP服务器，端口可能被占用!";
        return false;
    }
    // 端口绑定成功，配置路由
    httpServer.route("/ping",[](const QHttpServerRequest& req){
        (void) req;
        qDebug() << "[HTTP] 收到 ping 请求";
        return "pong";
    });

    // lambda表达式的参数就是收到的请求，也就是一个QHttpServerRequest对象;返回值是一个QHttpServerResponse对象
    httpServer.route("/service/user/get_user_info",[=](const QHttpServerRequest& req){
        return this->getUserInfo(req);
    });
    return true;
}

QHttpServerResponse HttpServer::getUserInfo(const QHttpServerRequest& req)
{
    // 1.解析请求,把req里的body经过pb反序列化
    bite_im::GetUserInfoReq pbReq;
    pbReq.deserialize(&serializer,req.body());
    LOG() << "[获取用户信息] requestId = " << pbReq.requestId() << ", chatSessionId = " << pbReq.sessionId();

    // 2.将反序列化好的内容构造成响应数据
    bite_im::GetUserInfoRsp pbResp;
    pbResp.setRequestId(pbReq.requestId());
    pbResp.setSuccess(true);
    pbResp.setErrmsg("");

    bite_im::UserInfo userInfo;
    userInfo.setUserId("123456");
    userInfo.setNickname("张三");
    userInfo.setPhone("18246262332");
    userInfo.setDescription("这是一个个性签名");
    userInfo.setAvatar(readFiletoQByteArray(":/resource/image/groupAvatar.jpg"));
    pbResp.setUserInfo(userInfo);

    // 3.将构造好的数据序列化并构造 HTTP 响应
    QByteArray body = pbResp.serialize(&serializer);

    // 老版本写法,QT已经不支持了
    // QHttpServerResponse httpResp(body,QHttpServerResponse::StatusCode::Ok);
    // httpResp.setHeader("Content-Type","application/x-protobuf");
    return QHttpServerResponse("application/x-protobuf",body);
}

///////////////////////////////////////////////////////////////
/// Websocket 服务器
///////////////////////////////////////////////////////////////

WebSocketServer* WebSocketServer::instance = nullptr;

WebSocketServer *WebSocketServer::getInstance()
{
    if(instance == nullptr)
    {
        instance = new WebSocketServer();
    }
    return instance;
}

bool WebSocketServer::init()
{
    // 1.连接信号槽
    connect(&webSocketServer,&QWebSocketServer::newConnection,this,[=](){
        // 建立连接成功之后
        qDebug() << "[websocket]连接成功！";

        // 获取到用于通信的socket，newPendingconnection就类似于原生的accept
        QWebSocket* socket = webSocketServer.nextPendingConnection();

        // 针对这个websocket对象，处理剩余的信号连接
        connect(socket, &QWebSocket::disconnected, this, [=]() {
            qDebug() << "[websocket] 连接断开！";
        });

        connect(socket, &QWebSocket::errorOccurred, this, [=](QAbstractSocket::SocketError error) {
            qDebug() << "[websocket] 连接出错！ " << error;
        });

        connect(socket, &QWebSocket::textMessageReceived, this, [=](const QString& message) {
            qDebug() << "[websocket] 收到文本数据！ message=" << message;
        });

        connect(socket, &QWebSocket::binaryMessageReceived, this, [=](const QByteArray& byteArray) {
            qDebug() << "[websocket] 收到二进制数据！ " << byteArray.length();
        });
    });

    // 2.绑定端口,启动服务
    bool ok = webSocketServer.listen(QHostAddress::Any,8001);
    return ok;
}













