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
    void initWebSocket();

    ~DataCenter();
private:
    DataCenter();
    static DataCenter* instance;

    // 列出 DataCenter 中要组织的所有数据

    // 客户端登录到服务器的 id
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

    // 通过网络将消息发送出去
    void sendTextMessageAsync(const QString &chatSessionId,const QString& message);
    void sendImageMessageAsync(const QByteArray& fileContent,const QString& chatSessionId);
    void sendFileMessageAsync(const QString& chatSessionId, const QString& fileName, const QByteArray& content);

    // 通过网络请求修改用户昵称
    void changeNickNameAsync(const QString& nickname);
    void resetNickName(const QString &nickname);
    // 通过网络请求修改用户签名
    void changeDesciptionAsync(const QString& desciption);
    void resetDescription(const QString& description);
    // 获取验证码
    void getVerifyCodeAsync(const QString& phone);
    void resetVerifyCodeId(const QString& verifyCodeId);
    const QString& getVerifyCodeId();
    // 修改手机号
    void changePhoneAsync(const QString& phone,const QString& verifyCodeId,const QString& verifyCode);
    void resetPhoneNumber(const QString& phone);
    // 修改用户头像
    void changeAvatarAsync(const QByteArray& imageBytes);
    void resetAvatar(const QByteArray& imageBytes);

    // 删除好友
    void deleteFriendAsync(const QString& userId);
    void removeFriend(const QString& userId);

    // 发送好友申请
    void addFriendApplyAsync(const QString& userId);
    // 同意好友申请
    void acceptFriendApplyAsync(const QString& userId);
    UserInfo removeFromApplyList(const QString& userId);
    // 拒绝好友申请
    void rejectFriendApplyAsync(const QString& userId);

    // 创建群聊
    void createGroupSessionAsync(const QList<QString>& userIdList);
    // 获取当前会话成员列表
    void getGroupSessionMemberListAsync(const QString& chatSessionId);
    void resetGroupMemberList(const QString& chatSessionId,const QList<bite_im::UserInfo>& memberList);
    QList<UserInfo>* getgroupSessionMemberList(const QString& chatSessionId);

    // 搜索好友
    void searchFriendAsync(const QString& text);
    void resetSearchFriendResult(const QList<bite_im::UserInfo>& searchResult);
    QList<UserInfo>* getSearchResultList();

    // 搜索历史消息
    void searchMessageListAsync(const QString& searchKey);
    void searchMessageListByTimeAsync(const QDateTime& beginTime,const QDateTime& endTime);
    void resetSearchMessageResult(const QList<bite_im::MessageInfo>& messageList);
    QList<Message>* getSearchMessageResult();

    // 用户名登录注册
    void userLoginAsync(const QString& username,const QString& password);
    void resetLoginSessionId(const QString& loginSessionId);
    void userRegisterAsync(const QString& username,const QString& password);
    // 手机号登录注册
    void phoneLoginAsync(const QString& phone,const QString& verifyCode);
    void phoneRegisterAsync(const QString& phone,const QString& verifyCode);


    // 获取单个文件
    void getSingleFileAsync(const QString& fileId);

    ////////////////////////////////////////////////////////////////////////////
    /// 辅助函数
    ////////////////////////////////////////////////////////////////////////////
    // 通过sessionId查找会话信息
    chatSessionInfo* findSessionById(const QString& chatSessionId);
    // 通过userId查找会话信息
    chatSessionInfo* findChatSessionByUserId(const QString& userId);
    // 把指定的会话信息置顶
    void topChatSessionInfo(chatSessionInfo& chatSessionInfo);

    // 通过userId找好友
    UserInfo* findFriendById(const QString& userId);

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
    void changePhoneNumberDone();
    void getVerifyCodeDone();
    void changeAvatarDone();
    void deleteFriendDone();
    void clearCurrentSession();
    void addFriendApplyDone();
    void receiveFriendApplyDone();
    void acceptFriendApplyDone();
    void acceptFriendProcessDone(const QString& nickName,bool agree);
    void rejectFriendApplyDone();
    void createGroupSessionDone();
    void receiveCreateSessionDone();
    void getGroupMemberListDone(const QString& chatSessionId);
    void searchFriendDone();
    void searchMessageResultDone();
    void userLoginDone(bool ok,const QString& reason);
    void userRegisterDone(bool ok,const QString& reason);
    void phoneLoginDone(bool ok,const QString& reason);
    void phoneRegisterDone(bool ok,const QString& reason);
    void getSingleFileDone(const QString& fileId,const QByteArray& fileContent);
};

} // end namespace
#endif // DATACENTER_H
