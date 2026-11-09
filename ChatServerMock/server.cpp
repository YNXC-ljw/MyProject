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

/////////////////////////////////////////////////////////////
//// 一些辅助函数
/////////////////////////////////////////////////////////////

// 生成默认的 UserInfo 对象
bite_im::UserInfo makeUserInfo(int index, const QByteArray& avatar) {
    bite_im::UserInfo userInfo;
    userInfo.setUserId(QString::number(1000 + index));
    userInfo.setNickname("张三" + QString::number(index));
    userInfo.setDescription("个性签名" + QString::number(index));
    userInfo.setPhone("18612345678");
    userInfo.setAvatar(avatar);
    return userInfo;
}

// 生成默认的 MessageInfo 对象，文本消息
bite_im::MessageInfo makeTextMessageInfo(int index, const QByteArray& avatar)
{
    bite_im::MessageInfo messageInfo;
    messageInfo.setMessageId(QString::number(3000 + index));
    messageInfo.setChatSessionId(QString::number(2000 + index));
    messageInfo.setTimestamp(getTime());
    messageInfo.setSender(makeUserInfo(index, avatar));

    bite_im::StringMessageInfo stringMessageInfo;
    stringMessageInfo.setContent("这是一条消息内容" + QString::number(index));

    bite_im::MessageContent messageContent;
    messageContent.setMessageType(bite_im::MessageTypeGadget::MessageType::STRING);
    messageContent.setStringMessage(stringMessageInfo);
    messageInfo.setMessage(messageContent);
    return messageInfo;
}

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

    // 获取好友列表
    httpServer.route("/service/friend/get_friend_list",[=](const QHttpServerRequest& req){
        return this->getFriendList(req);
    });

    // 获取会话列表
    httpServer.route("/service/friend/get_chat_session_list",[=](const QHttpServerRequest& req){
        return this->getChatSessionList(req);
    });

    // 获取好友申请列表
    httpServer.route("/service/friend/get_pending_friend_events",[=](const QHttpServerRequest& req){
        return this->getApplyList(req);
    });
    return true;
}

QHttpServerResponse HttpServer::getUserInfo(const QHttpServerRequest& req)
{
    // 1.解析请求,把req里的body经过pb反序列化
    bite_im::GetUserInfoReq pbReq;
    pbReq.deserialize(&serializer,req.body());
    LOG() << "[获取用户信息] requestId = " << pbReq.requestId() << ", loginSessionId = " << pbReq.sessionId();

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

QHttpServerResponse HttpServer::getFriendList(const QHttpServerRequest &req)
{
    // 1.解析请求
    bite_im::GetFriendListReq pbReq;
    pbReq.deserialize(&serializer,req.body());
    LOG() << "[获取好友列表] requestId = " << pbReq.requestId() << ", loginSessionId = " << pbReq.sessionId();

    // 2.将反序列化好的请求，构造成响应数据
    bite_im::GetFriendListRsp pbResp;
    pbResp.setRequestId(pbReq.requestId());
    pbResp.setSuccess(true);
    pbResp.setErrmsg("");

    // 3.构造假数据添加到相应数据中
    QByteArray avatar = readFiletoQByteArray(":/resource/image/groupAvatar.jpg");
    QList<bite_im::UserInfo> friendList;
    for(int i = 0;i < 30;i++)
    {
        bite_im::UserInfo userInfo = makeUserInfo(i + 1,avatar);
        friendList.push_back(userInfo);
    }
    pbResp.setFriendList(std::move(friendList));

    // 4.序列化构造好的数据
    QByteArray body = pbResp.serialize(&serializer);
    return QHttpServerResponse("application/x-protobuf",body);
}

QHttpServerResponse HttpServer::getChatSessionList(const QHttpServerRequest &req)
{
    // 1.解析请求
    bite_im::GetChatSessionListReq pbReq;
    pbReq.deserialize(&serializer,req.body());
    LOG() << "[获取会话列表] requestId = " << pbReq.requestId() << ", loginSessionId = " << pbReq.sessionId();

    // 2.构造响应数据
    bite_im::GetChatSessionListRsp pbResp;
    pbResp.setRequestId(pbReq.requestId());
    pbResp.setErrmsg("");
    pbResp.setSuccess(true);

    // 3.构造假数据填充会话列表
    QByteArray avatar = readFiletoQByteArray(":/resource/image/groupAvatar.jpg");
    QList<bite_im::ChatSessionInfo> chatSessionList;
    for (int i = 0; i < 30; ++i) {
        bite_im::ChatSessionInfo chatSessionInfo;
        chatSessionInfo.setChatSessionId(QString::number(2000 + i));
        chatSessionInfo.setChatSessionName("会话" + QString::number(i));
        chatSessionInfo.setSingleChatFriendId(QString::number(1000 + i));
        chatSessionInfo.setAvatar(avatar);
        bite_im::MessageInfo messageInfo = makeTextMessageInfo(i, avatar);

        chatSessionInfo.setPrevMessage(messageInfo);
        chatSessionList.push_back(chatSessionInfo);
    }
    pbResp.setChatSessionInfoList(std::move(chatSessionList));

    // 4.将构造好的数据序列化后返回给客户端响应
    QByteArray body = pbResp.serialize(&serializer);
    return QHttpServerResponse("application/x-protobuf",body);
}

QHttpServerResponse HttpServer::getApplyList(const QHttpServerRequest &req)
{
    // 1.解析请求
    bite_im::GetPendingFriendEventListReq pbReq;
    pbReq.deserialize(&serializer, req.body());
    LOG() << "[REQ 获取好友申请列表] requestId=" << pbReq.requestId() << ", loginSessionId=" << pbReq.sessionId();

    // 2.构造响应
    bite_im::GetPendingFriendEventListRsp pbResp;
    pbResp.setRequestId(pbReq.requestId());
    pbResp.setSuccess(true);
    pbResp.setErrmsg("");

    // 3.循环构造出 event 对象，构造出整个结果数组
    QByteArray avatar = readFiletoQByteArray(":/resource/image/avatar.jpg");
    QList<bite_im::FriendEvent> eventList;
    for (int i = 0; i < 5; ++i) {
        bite_im::FriendEvent friendEvent;
        friendEvent.setEventId(""); // 此处不再使用这个 eventId，直接设为 ""
        friendEvent.setSender(makeUserInfo(i, avatar));

        eventList.push_back(friendEvent);
    }
    pbResp.setEvent(eventList);

    // 序列化成字节数组
    QByteArray body = pbResp.serialize(&serializer);
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













