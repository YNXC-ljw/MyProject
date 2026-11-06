#include "netclient.h"
#include <QNetworkReply>
#include<QUuid>
#include "../model/data.h"
#include "../model/datacenter.h"

namespace network{

NetClient::NetClient(model::DataCenter *dataCenter)
    :dataCenter(dataCenter)
{
    // 程序启动即调用
    initWebSocket();
}

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
        // 后续的内容处理，后面再写
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
void NetClient::getMyself(const QString &chatSessionId)
{
    // 1.构造出 HTTP 请求 body 部分
    bite_im::GetUserInfoReq req;
    req.setRequestId(makeRequestId());
    req.setSessionId(chatSessionId);
    QByteArray body = req.serialize(&serializer);
    LOG() << "[获取个人信息] requestId = " << req.requestId() << ", chatSessionId = " << req.sessionId();

    // 2.构造 HTTP 请求，并将请求发出去
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

        // -05 继续处理后续逻辑......

    });
}



} // end namespace










