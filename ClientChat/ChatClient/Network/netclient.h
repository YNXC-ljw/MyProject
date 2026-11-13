#ifndef NETCLIENT_H
#define NETCLIENT_H

#include <QNetworkAccessManager>
#include <QWebSocket>
#include <QObject>
#include <QProtobufSerializer>
#include <QNetworkReply>
#include "../model/data.h"

// 此处为了避免循环包含头文件，就需要前置声明来解决问题
namespace model{
    class DataCenter;
    class Data;
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

//////////////////////////////////////////////////////////////////////////////////
/// WebSocket 服务器相关操作
//////////////////////////////////////////////////////////////////////////////////
    // 初始化websocket
    void initWebSocket();
    // 发送验证身份请求(序列化操作)
    void sentAuth();
    void handleWsResponse(const bite_im::NotifyMessage &notifyMessage);
    void handleWsMessage(const model::Message& message);
    void receiveMessage(const QString& chatSessionId);
//////////////////////////////////////////////////////////////////////////////////
/// HTTP 服务器相关操作
//////////////////////////////////////////////////////////////////////////////////
    // 测试HTTP连通性
    void ping();
    // 生成请求Id
    QString makeRequestId();

    // 封装发送请求逻辑
    QNetworkReply* sendHttpRequest(const QString& apiPaths,const QByteArray& body);
    // 封装处理响应的逻辑（包括判定 HTTP 正确性，反序列化，判定业务上的正确性）
    // 由于不同的api，返回的pb对象结构，不同，为了让一个函数能处理多种不同类型，需要使用 模板(但是模板规定生命与定义不能分离)
    template <typename T>
    std::shared_ptr<T> handleHttpResponse(QNetworkReply* httpResp,bool *ok,QString* reason)// 通过输出型参数将信息带出去再打印日志
    {
        // 1.检查HTTP请求是否出错
        if(httpResp->error() != QNetworkReply::NoError)
        {
            *ok = false;
            *reason = httpResp->errorString();
            httpResp->deleteLater();
            return std::shared_ptr<T>();
        }

        // 2.获取到响应的 body
        QByteArray respBody = httpResp->readAll();

        // 3.将 body 进行反序列化
        std::shared_ptr<T> respObj = std::make_shared<T>();
        respObj->deserialize(&serializer,respBody);

        // 4.判断业务是否正确
        if(!respObj->success())
        {
            *ok = false;
            *reason = respObj->errmsg();
            httpResp->deleteLater();
            return std::shared_ptr<T>();
        }

        // 5.释放掉 httpResp 对象
        httpResp->deleteLater();
        *ok = true;
        return respObj;
    }
    // 通过这个函数进一步完成网络通信内容
    void getMyself(const QString& loginSessionId);
    // 获取好友列表
    void getFriendList(const QString& loginSessionId);
    // 获取会话列表
    void getChatSessionList(const QString& loginSessionId);
    // 获取好友申请列表
    void getApplyList(const QString& loginSessionId);
    // 获取选中会话的历史消息
    void getRecentMessage(const QString& loginSessionId,const QString& chatSessionId,bool updateUI);

    // 发送文本消息
    void sendMessage(const QString& loginSessionId,const QString& chatSessionId,model::MessageType messageType,
                     const QByteArray& message,const QString& extroInfo);

    // 修改用户昵称
    void changeNickName(const QString& loginSessionId,const QString& nickname);
    // 修改用户签名
    void changeDescription(const QString& loginSessionId,const QString& description);
    // 获取短信验证码
    void getVerifyCode(const QString& phone);
    // 修改手机号码
    void changePhone(const QString& loginSessionId,const QString& phone,const QString& verifyCodeId,const QString& verifyCode);
    // 修改头像
    void changeAvatar(const QString& loginSessionId,const QByteArray& imageBytes);

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
