#include "netclient.h"
#include <QNetworkReply>
#include<QUuid>
#include "../model/data.h"
#include "../model/datacenter.h"

using namespace model;
namespace network{

NetClient::NetClient(model::DataCenter *dataCenter)
    :dataCenter(dataCenter)
{
    // 程序启动即调用
    initWebSocket();
}

///////////////////////////////////////////////////////////////////////////////////////////
/// WebSocket 客户端
///////////////////////////////////////////////////////////////////////////////////////////

// 初始化Websocket
void NetClient::initWebSocket()
{
    // 1.准备好所有需要的信号槽
    connect(&webSocketClient,&QWebSocket::connected,this,[=](){
        LOG() << "WebSocket 连接成功！";

        // 服务器连接成功后，要调用函数发送身份认证
        sentAuth();
    });
    connect(&webSocketClient, &QWebSocket::disconnected, this, [=]() {
        LOG() << "WebSocket 连接断开！";
    });

    connect(&webSocketClient, &QWebSocket::errorOccurred, this, [=](QAbstractSocket::SocketError error) {
        LOG() << "WebSocket 连接出错！" << error;
    });

    connect(&webSocketClient, &QWebSocket::textMessageReceived, this, [=](const QString& message) {
        LOG() << "WebSocket 收到文本消息！" << message;
    });

    connect(&webSocketClient, &QWebSocket::binaryMessageReceived, this, [=](const QByteArray& byteArray) {
        LOG() << "WebSocket 收到二进制消息！" << byteArray.length();
        bite_im::NotifyMessage notifyMessage;
        notifyMessage.deserialize(&serializer,byteArray);
        handleWsResponse(notifyMessage);
    });

    // 2.和服务器真正建立连接
    webSocketClient.open(WEBSOCKET_URL);
}

// 发送身份认证请求
void NetClient::sentAuth()
{
    bite_im::ClientAuthenticationReq req;
    req.setRequestId(makeRequestId());
    req.setSessionId(dataCenter->getLoginSessionId());
    QByteArray message = req.serialize(&serializer);
    webSocketClient.sendBinaryMessage(message);
    LOG() << "[websocket身份认证] requestId = " << req.requestId() << ", loginSessionId = " << req.sessionId();
}

void NetClient::handleWsResponse(const bite_im::NotifyMessage &notifyMessage)
{

}

///////////////////////////////////////////////////////////////////////////////////////////
/// HTTP 客户端
///////////////////////////////////////////////////////////////////////////////////////////

// 检验HTTP连通性
void NetClient::ping()
{
    // 1.制作请求
    QNetworkRequest httpReq;
    httpReq.setUrl(HTTP_URL + "/ping");

    // 2.获取响应(响应和请求不同，请求是立即发送的，但是响应回来需要时间 )
    QNetworkReply* httpResp = httpClient.get(httpReq);
    connect(httpResp,&QNetworkReply::finished,this,[=](){
        // 此时说明响应已经回来
        if(httpResp->error() != QNetworkReply::NoError) // 确认网络请求没有出错
        {
            LOG() << "HTTP请求失败!" << httpResp->errorString();
            httpResp->deleteLater();
            return;
        }
        // 获取响应的body
        QByteArray body = httpResp->readAll();
        LOG() << "响应内容：" << body;
        httpResp->deleteLater();
    });
}

// 生成唯一Id
QString NetClient::makeRequestId()
{
    // 基本要求：确保每个请求的 id 都是唯一的（UUID）
    return "R" + QUuid::createUuid().toString().sliced(25,12); // 只获取后12个
}

QNetworkReply *NetClient::sendHttpRequest(const QString &apiPaths, const QByteArray &body)
{
    QNetworkRequest httpReq;
    httpReq.setUrl(QUrl(HTTP_URL + apiPaths));
    httpReq.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-protobuf");
    // 返回响应
    return httpClient.post(httpReq,body);
}

// 通过这个函数，进一步完成网络通信内容
void NetClient::getMyself(const QString &loginSessionId)
{
    // 1.构造出 HTTP 请求 body 部分
    bite_im::GetUserInfoReq req;
    req.setRequestId(makeRequestId());
    req.setSessionId(loginSessionId);
    QByteArray body = req.serialize(&serializer);
    LOG() << "[获取个人信息] 发送请求 requestId = " << req.requestId() << ", chatSessionId = " << req.sessionId();

    // 2.构造 HTTP 请求，并将请求发出去,通过信号槽异步处理返回的响应
    QNetworkReply* httpResp = sendHttpRequest("/service/user/get_user_info",body);

    // 3.连接信号槽，获取响应
    connect(httpResp,&QNetworkReply::finished,this,[=](){
        // -01 判定 HTTP 是否出错
        // if(httpResp->error() != QNetworkReply::NoError)
        // {
        //     LOG() << "HTTP 请求出错!" << httpResp->errorString();
        //     httpResp->deleteLater();
        //     return;
        // }

        // // -02 拿到响应的body
        // QByteArray respBody = httpResp->readAll();

        // // -03 针对 body 进行反序列化
        // bite_im::GetUserInfoRsp resp;
        // resp.deserialize(&serializer,respBody);

        // // -04 判定业务是否出错
        // if(!resp.success())
        // {
        //     LOG() << "requestId = " << resp.requestId() << "errmsg : " << resp.errmsg();
        //     httpResp->deleteLater();
        //     return;
        // }
        // a) 先处理响应对象
        bool ok = false;
        QString reason;
        auto myselfResp = handleHttpResponse<bite_im::GetUserInfoRsp>(httpResp,&ok,&reason);

        // b) 判定响应是否正确并打印日志
        if(!ok)
        {
            LOG() << "[获取个人信息] 出错！reason = " << reason;
            return;
        }
        // c) 把响应的数据刷新到 dataCenter 中
        dataCenter->resetMyself(myselfResp);

        // d) 通知调用逻辑，响应已经处理完成
        emit dataCenter->getMyselfDone();
        // e) 打印日志
        LOG() << "[获取个人信息] 处理响应 requestId = " << myselfResp->requestId();

    });
}

void NetClient::getFriendList(const QString &loginSessionId)
{
    // 1.构造 HTTP 请求 body
    bite_im::GetFriendListReq req;
    req.setSessionId(loginSessionId);
    req.setRequestId(makeRequestId());
    QByteArray body = req.serialize(&serializer);
    LOG() << "[获取好友列表] 发送请求 requestId = " << req.requestId() << ", chatSessionId = " << req.sessionId();

    // 2.构造 HTTP 请求并将请求发送出去,通过信号槽异步处理返回的响应
    QNetworkReply* httpResp = sendHttpRequest("/service/friend/get_friend_list",body);

    // 3.连接信号槽异步处理返回的响应
    connect(httpResp,&QNetworkReply::finished,this,[=](){
        // a) 先处理响应对象
        bool ok = false;
        QString reason;
        auto friendListResp = handleHttpResponse<bite_im::GetFriendListRsp>(httpResp,&ok,&reason);

        // b) 判断响应是否正确并打印日志
        if(!ok)
        {
            LOG() << "[获取好友列表] 出错! reason = " << reason;
            return;
        }

        // c) 把响应数据刷到 Datacenter 中
        dataCenter->resetFriendList(friendListResp);

        // d) 发送信号通知
        emit dataCenter->getFriendListDone();

        // e) 打印日志
        LOG() << "[获取好友列表] 处理响应 requestId = " << friendListResp->requestId();
    });
}

void NetClient::getChatSessionList(const QString &loginSessionId)
{
    // 1.构造请求
    bite_im::GetChatSessionListReq req;
    req.setSessionId(loginSessionId);
    req.setRequestId(makeRequestId());
    QByteArray body = req.serialize(&serializer);
    LOG() << "[获取会话列表] 发送请求 requestId = " << req.requestId() << ", chatSessionId = " << req.sessionId();

    // 2.发送请求并获取响应
    QNetworkReply* httpResp = sendHttpRequest("/service/friend/get_chat_session_list",body);

    // 3.通过信号槽异步处理返回的响应
    connect(httpResp,&QNetworkReply::finished,this,[=](){
        // a) 先处理响应对象
        bool ok = false;
        QString reason;
        auto chatSessionListResp = handleHttpResponse<bite_im::GetChatSessionListRsp>(httpResp,&ok,&reason);

        // b) 判定响应是否正确并打印日志
        if(!ok)
        {
            LOG() << "[获取会话列表] 出错! reason = " << reason;
            return;
        }

        // c) 把响应会话列表对象刷新到 DataCenter 中
        dataCenter->resetChatSessionList(chatSessionListResp);

        // d) 发送信号
        emit dataCenter->getChatSessionListDone();

        // e) 打印日志
        LOG() << "[获取会话列表] 处理响应 requestId = " << chatSessionListResp->requestId();
    });
}

void NetClient::getApplyList(const QString &loginSessionId)
{
    bite_im::GetPendingFriendEventListReq req;
    req.setRequestId(makeRequestId());
    req.setSessionId(loginSessionId);
    QByteArray body = req.serialize(&serializer);
    LOG() << "[获取好友申请列表] 发送请求 requestId=" << req.requestId() << ", loginSessionId=" << loginSessionId;

    // 2. 发送 http 请求
    QNetworkReply* httpResp = sendHttpRequest("/service/friend/get_pending_friend_events", body);

    // 3. 处理响应
    connect(httpResp, &QNetworkReply::finished, this, [=]() {
        // a) 解析响应
        bool ok = false;
        QString reason;
        auto recentMessageResp = handleHttpResponse<bite_im::GetPendingFriendEventListRsp>(httpResp, &ok, &reason);

        // b) 判定结果是否出错
        if (!ok) {
            LOG() << "[获取好友申请列表] 失败! reason = " << reason;
            return;
        }

        // c) 拿到的数据，写入到 DataCenter 中
        dataCenter->resetApplyList(recentMessageResp);

        // d) 通知界面，处理完毕
        emit dataCenter->getApplyListDone();

        // e) 打印日志
        LOG() << "[获取好友申请列表] 处理响应完成! requestId=" << recentMessageResp->requestId();
    });

}

void NetClient::getRecentMessage(const QString &loginSessionId, const QString &chatSessionId)
{
    // 1.构造HTTP请求body
    bite_im::GetRecentMsgReq req;
    req.setChatSessionId(chatSessionId);
    req.setRequestId(makeRequestId());
    req.setMsgCount(50);
    req.setSessionId(loginSessionId);
    QByteArray body = req.serialize(&serializer);
    LOG() << "[获取最近消息] 发送请求 requestId = " << req.requestId() << ", chatSessionId" << req.chatSessionId();

    // 2.发送请求并返回响应
    QNetworkReply* httpResp = sendHttpRequest("/service/message_storage/get_recent",body);

    // 3.处理服务器返回的响应
    connect(httpResp,&QNetworkReply::finished,this,[=](){
        // a) 解析响应，反序列化
        bool ok = false;
        QString reason;
        auto pbResp = handleHttpResponse<bite_im::GetRecentMsgRsp>(httpResp,&ok,&reason);

        // b) 判定响应是否出错
        if(!ok)
        {
            LOG() << "[获取最近消息] 失败！ reason = " << reason;
        }

        // c) 将反序列化好的数据存到 DataCenter 中
        dataCenter->resetRecentMessageList(chatSessionId,pbResp);

        // d) 通知界面处理完毕,可以渲染界面
        emit dataCenter->getRecentMessageDone(chatSessionId);

        // e) 打印日志
        LOG() << "[获取最近消息] 响应处理完毕！ requestId = " << pbResp->requestId();
    });
}

// 此处的 extraInfo，可以用来传递 "扩展信息"，尤其是对于文件消息来说，通过这个字段表示 "文件名"
// 其他类型的消息暂时不涉及，就直接设为 ""，如果后续有消息类型需要，都可以给这个参数，灵活的扩充

void NetClient::sendMessage(const QString &loginSessionId, const QString &chatSessionId, MessageType messageType,
                            const QByteArray &message,const QString& extraInfo)
{
    // 1.通过pb构造请求
    bite_im::NewMessageReq req;
    req.setChatSessionId(chatSessionId);
    req.setSessionId(loginSessionId);
    req.setRequestId(makeRequestId());

    // 构造messageContent
    bite_im::MessageContent messageContent;
    if(messageType == MessageType::TEXT_TYPE)
    {
        messageContent.setMessageType(bite_im::MessageTypeGadget::MessageType::STRING);

        bite_im::StringMessageInfo stringMessageInfo;
        stringMessageInfo.setContent(message);
        messageContent.setStringMessage(stringMessageInfo);
    }else if(messageType == MessageType::IMAGE_TYPE){
        messageContent.setMessageType(bite_im::MessageTypeGadget::MessageType::IMAGE);

        bite_im::StringMessageInfo stringMessageInfo;
        stringMessageInfo.setContent(message);
        messageContent.setStringMessage(stringMessageInfo);
    } else if (messageType == MessageType::IMAGE_TYPE) {
        messageContent.setMessageType(bite_im::MessageTypeGadget::MessageType::IMAGE);

        bite_im::ImageMessageInfo imageMessageInfo;
        imageMessageInfo.setFileId("");      // fileId 是文件在服务器存储的时候，生成的 id，此时还无法获取到，暂时填成 ""
        imageMessageInfo.setImageContent(message);
        messageContent.setImageMessage(imageMessageInfo);
    } else if (messageType == MessageType::FILE_TYPE) {
        messageContent.setMessageType(bite_im::MessageTypeGadget::MessageType::FILE);

        bite_im::FileMessageInfo fileMessageInfo;
        fileMessageInfo.setFileId("");       // fileId 是文件在服务器存储的时候，生成的 id，此时还无法获取到，暂时填成 ""
        fileMessageInfo.setFileSize(message.size());
        fileMessageInfo.setFileName(extraInfo);
        fileMessageInfo.setFileContents(message);
        messageContent.setFileMessage(fileMessageInfo);
    } else if (messageType == MessageType::SPEECH_TYPE) {
        messageContent.setMessageType(bite_im::MessageTypeGadget::MessageType::SPEECH);

        bite_im::SpeechMessageInfo speechMessageInfo;
        speechMessageInfo.setFileId("");     // fileId 是文件在服务器存储的时候，生成的 id，此时还无法获取到，暂时填成 ""
        speechMessageInfo.setFileContents(message);
        messageContent.setSpeechMessage(speechMessageInfo);
    } else {
        LOG() << "错误的消息类型! messageType=" << messageType;
    }
    req.setMessage(messageContent);

    // 序列化
    QByteArray body = req.serialize(&serializer);
    LOG() << "[发送消息] 发送请求 requestId = " << req.requestId() << ", loginSessionId = " << req.sessionId() <<
        ", chatSessionId = " << req.chatSessionId() << ", messageType = " << req.message().messageType();
    // 2.发送请求并接收返回的响应
    QNetworkReply* httpResp = sendHttpRequest("/service/message_transmit/new_message",body);

    connect(httpResp,&QNetworkReply::finished,this,[=](){
        // a) 针对响应结果进行解析
        bool ok = false;
        QString reason;
        auto pbResp = this->handleHttpResponse<bite_im::NewMessageRsp>(httpResp, &ok, &reason);

        // b) 判定响应是否正确
        if (!ok) {
            LOG() << "[发送消息] 处理出错! reason=" << reason;
            return;
        }

        // c) 此处只是需要记录 "成功失败"，不需要把内容写入到 DataCenter 中。

        // d) 通知调用者，响应处理完毕
        emit dataCenter->sendMessageDone(messageType, message, extraInfo);

        // e) 打印日志
        LOG() << "[发送消息] 响应处理完毕! requestId=" << pbResp->requestId();

    });
}



} // end namespace










