#ifndef DATACENTER_H
#define DATACENTER_H

#include <QWidget>
#include "data.h"
#include "../Network/netclient.h"

namespace model {

class DataCenter : public QObject
{
    Q_OBJECT
public:
    static DataCenter* getInstance();

    ~DataCenter();
private:
    DataCenter();
    static DataCenter* instance;

    // 列出 DataCenter 中要组织的所有数据

    // 客户端登录到服务器的会话id
    QString loginSessionId = "";
    // 当前用户信息
    UserInfo* myself = nullptr;
    // 当前用户的好友列表
    QList<UserInfo>* friendList = nullptr;
    // 会话列表
    QList<chatSessionInfo>* chatSessionList = nullptr;
    // 记录当前选中的会话
    QString currentChatSessionId = "";
    // 每个会话中有哪些成员(主要针对群聊), key = chatSessionId; value = 用户列表
    QHash<QString,QList<UserInfo>>* memberList = nullptr;
    // 待处理的好友申请列表
    QList<UserInfo>* applyList = nullptr;
    // 每个会话的最近消息列表. key = chatSesionId; value = 消息列表
    QHash<QString,QList<Message>>* recentMessages = nullptr;
    // 存储每个会话，未读消息的个数, key = chatSessionId; value = 未读消息个数
    QHash<QString,int>* unReadMessageCount = nullptr;
    // 用户的好友搜索结果
    QList<UserInfo>* searchFriendResult = nullptr;
    // 历史消息搜索结果
    QList<Message>* searchMessageResult = nullptr;
    // 短信验证码的验证 id(获取验证码时服务器会给客户端生成一个codeId云服务器厂商会给客户端发送验证码code
    // 你需要输入正确的code还要带上服务器给客户端的codeId才能验证，因为客户端可能会有很多个，要通过键值对的方式验证)
    QString currentVerifyCodeId = "";

    // 让 DataCenter持有NetClient
    network::NetClient netClient;

public:
    ////////////////////////////////////////////////////////////////////////////
    /// 辅助函数
    ////////////////////////////////////////////////////////////////////////////
    // 初始化数据文件
    void initDataFile();
    // 存储数据到文件中
    void saveDataToFile();
    // 将文件数据加载到内存中
    void loadDataFile();
    // 清除当前会话的未读消息数
    void clearUnReadCount(const QString& chatSessionId);
    // 增加未读消息数
    void addUnReadCount(const QString& chatSessionId);
    // 获取未读消息数
    int getUnReadCount(const QString& chatSessionId);

    // 获取当前登录会话Id
    const QString& getLoginSessionId() const
    {
        return loginSessionId;
    }

    // 测试网络连通性
    void ping() {netClient.ping();}

    //////////////////////////////////////////////////////////////////////////////
    /// 核心函数
    //////////////////////////////////////////////////////////////////////////////
    // 通过网络获取用户信息(该函数只负责把HTTP请求发出去，什么时候回来就不管了，通过槽函数接收返回的消息)
    void getMyselfAsync();// 异步(从服务器中取出)
    UserInfo* getMyself();// 同步(本地内存中取出)
    // 将通信响应对象信息刷新到 dataCenter 中
    void resetMyself(std::shared_ptr<bite_im::GetUserInfoRsp> resp);

    // 获取好友列表
    QList<UserInfo>* getFriendList();// 通过内存获取
    void getFriendListAsync();// 通过网络获取
    // 将通信响应好友列表对象刷新到dataCenter中
    void resetFriendList(std::shared_ptr<bite_im::GetFriendListRsp> resp);

    // 获取会话列表
    QList<chatSessionInfo>* getChatSessionList();// 通过内存获取
    void getChatSessionListAsync();// 通过服务器获取
    // 将通讯响应会话列表对象刷新到 DataCenter 中
    void resetChatSessionList(std::shared_ptr<bite_im::GetChatSessionListRsp> resp);

    // 获取好友申请列表
    QList<UserInfo> *getApplyList();
    void getApplyListAsync();
    void resetApplyList(std::shared_ptr<bite_im::GetPendingFriendEventListRsp> resp);

    // 获取最近消息列表
    QList<Message>* getRecentMessageList(const QString& chatSessionId);// 本地获取
    void getRecentMessageAsync(const QString& chatSessionId,bool updateUI);//网络获取
    void resetRecentMessageList(const QString& chatSessionId,std::shared_ptr<bite_im::GetRecentMsgRsp> resp);

    // 通过网络将文本消息发送出去
    void sendTextMessageAsync(const QString &chatSessionId,const QString& message);

    // 通过网络请求修改用户昵称
    void changeNickNameAsync(const QString& nickname);
    void resetNickName(const QString &nickname);
    // 通过网络请求修改用户签名
    void changeDesciptionAsync(const QString& desciption);
    void resetDescription(const QString& description);

    ////////////////////////////////////////////////////////////////////////////
    /// 辅助函数
    ////////////////////////////////////////////////////////////////////////////
    // 通过sessionId查找会话信息
    chatSessionInfo* findSessionById(const QString& chatSessionId);
    // 通过userId查找会话信息
    chatSessionInfo* findChatSessionByUserId(const QString& userId);
    // 把指定的会话信息置顶
    void topChatSessionInfo(chatSessionInfo& chatSessionInfo);

    // 设置当前被选中的会话是哪个
    QString& getCurrentChatSessionId();
    void setCurrentChatSessionId(const QString& chatSessionId);

    // 添加message到DataCenter中
    void addMessage(const Message& message);

signals:
    // 定义处理信号
    void getMyselfDone();
    void getFriendListDone();
    void getChatSessionListDone();
    void getApplyListDone();
    void getRecentMessageDone(const QString& chatSessionId);
    void getRecentMessageNoUIDone(const QString& chatSessionId);
    void sendMessageDone(MessageType messageType, const QByteArray& message, const QString& extraInfo);
    void updateLastMessage(const QString& currentChatSessionId);
    void receiveMessageDone(const Message& message);
    void changeNickNameDone();
    void changeDescptionDone();
};

} // end namespace
#endif // DATACENTER_H
