#include "datacenter.h"
#include <QStandardPaths>
#include <QFile>
#include <QDir>
#include <QJsonObject>

namespace model{
DataCenter* DataCenter::instance = nullptr;

DataCenter *DataCenter::getInstance()
{
    if(instance == nullptr)
    {
        instance = new DataCenter();
    }
    return instance;
}

DataCenter::~DataCenter()
{
    // 释放所有的成员
    // 此处不必判定 nullptr，直接 delete 即可！
    // C++ 标准中明确规定，针对 nullptr 进行 delete，是合法行为，不会有任何副作用.
    delete myself;
    delete friendList;
    delete chatSessionList;
    delete memberList;
    delete applyList;
    delete recentMessages;
    delete unReadMessageCount;
    delete searchFriendResult;
    delete searchMessageResult;

}

DataCenter::DataCenter() :netClient(this)
{
    // 此处只实例化这几个QHash类型，其他暂时不做处理
    memberList = new QHash<QString,QList<UserInfo>>();
    recentMessages = new QHash<QString,QList<Message>>();
    unReadMessageCount = new QHash<QString,int>();

}

void DataCenter::initDataFile()
{
    // 构造文件路径，使用主机的AppData存放数据
    QString basePaths = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString filePaths = basePaths  + "/ChatClient.json";
    LOG() << "filepaths: " << filePaths;

    QDir dir;
    if(!dir.exists(basePaths))
    { // 目录不存在，创建目录
        dir.mkpath(basePaths);
    }

    // 构造好路径之后以写的方式创建并打开文件，写入初始内容
    QFile file(filePaths);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        LOG() << "文件打开失败!" << file.errorString();
        return;
    }
    // 打开成功，写入初始内容
    QString data = "{\n\n}";
    file.write(data.toUtf8());
    file.close();
}

// 保存数据
void DataCenter::saveDataToFile()
{
    QString filePaths = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/ChatClient.json";

    QFile file(filePaths);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        LOG() << "文件打开失败！" << file.errorString();
        return;
    }

    // 按照 json 方式将数据写入文件,就可以把该对象当作map一样使用了
    QJsonObject jsonObj;
    jsonObj["loginSessionId"] = loginSessionId;

    QJsonObject jsonUnread;
    for (auto it = unReadMessageCount->begin(); it != unReadMessageCount->end(); ++it) {
        // 注意 Qt 的迭代器使用细节和 STL 略有差别. 此处不是使用 first / second 的方式
        jsonUnread[it.key()] = it.value();
    }
    jsonObj["unread"] = jsonUnread;

    // 把 json 写入文件了
    QJsonDocument jsonDoc(jsonObj);
    QString s = jsonDoc.toJson();
    file.write(s.toUtf8());

    // 关闭文件
    file.close();

}

// 加载文件数据
void DataCenter::loadDataFile()
{
    QString filePaths = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/ChatClient.json";

    QFile file(filePaths);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        LOG() << "打开文件失败！" << file.errorString();
        return;
    }

}

//////////////////////////////////////////////////////
/// 获取个人信息
//////////////////////////////////////////////////////
UserInfo *DataCenter::getMyself()
{
    return myself;
}

// 通过网络获取用户信息(该函数只负责把HTTP请求发出去，什么时候回来就不管了，通过槽函数接收返回的消息)
void DataCenter::getMyselfAsync()
{
    netClient.getMyself(loginSessionId);
}

// 将通信响应对象信息刷新到 dataCenter 中
void DataCenter::resetMyself(std::shared_ptr<bite_im::GetUserInfoRsp> resp)
{
    if(myself == nullptr)
    {
        myself = new UserInfo();
    }
    const bite_im::UserInfo &userInfo = resp->userInfo();
    myself->load(userInfo);
}
//////////////////////////////////////////////////////
/// 获取好友列表
//////////////////////////////////////////////////////
QList<UserInfo>* DataCenter::getFriendList()
{
    return friendList;
}

void DataCenter::getFriendListAsync()
{
    netClient.getFriendList(loginSessionId);
}

void DataCenter::resetFriendList(std::shared_ptr<bite_im::GetFriendListRsp> resp)
{
    if(friendList == nullptr)
    {
        friendList = new QList<UserInfo>();
    }

    // 清空原本已存在的好友列表
    friendList->clear();

    // 依此遍历响应 好友列表 并放入到 friendList 中
    auto& pbFriendList = resp->friendList();
    for(auto& f : pbFriendList)
    {
        UserInfo userInfo;
        userInfo.load(f);
        friendList->push_back(userInfo);
    }
}
//////////////////////////////////////////////////////
/// 获取会话列表
//////////////////////////////////////////////////////
QList<chatSessionInfo> *DataCenter::getChatSessionList()
{
    return chatSessionList;
}

void DataCenter::getChatSessionListAsync()
{
    return netClient.getChatSessionList(loginSessionId);
}

void DataCenter::resetChatSessionList(std::shared_ptr<bite_im::GetChatSessionListRsp> resp)
{
    if(chatSessionList == nullptr)
    {
        chatSessionList = new QList<chatSessionInfo>();
    }
    // 清空原有的列表
    chatSessionList->clear();

    // 依此遍历返回结果中的列表加入到 DataCenter 中
    auto pbChatSessionList = resp->chatSessionInfoList();
    for(auto &c : pbChatSessionList)
    {
        chatSessionInfo chatSessionInfo;
        chatSessionInfo.load(c);
        chatSessionList->push_back(chatSessionInfo);
    }
}
//////////////////////////////////////////////////////
/// 获取好友申请列表
//////////////////////////////////////////////////////
QList<UserInfo> *DataCenter::getApplyList()
{
    return applyList;
}

void DataCenter::getApplyListAsync()
{
    netClient.getApplyList(loginSessionId);
}

void DataCenter::resetApplyList(std::shared_ptr<bite_im::GetPendingFriendEventListRsp> resp)
{
    if (applyList == nullptr) {
        applyList = new QList<UserInfo>();
    }
    applyList->clear();

    auto& eventList = resp->event();
    for (auto& event : eventList) {
        UserInfo userInfo;
        userInfo.load(event.sender());
        applyList->push_back(userInfo);
    }

}

QList<Message> *DataCenter::getRecentMessageList(const QString &chatSessionId)
{
    if(!recentMessages->contains(chatSessionId))
    {
        return nullptr;
    }
    return &(*recentMessages)[chatSessionId];
}

void DataCenter::getRecentMessageAsync(const QString &chatSessionId)
{
    netClient.getRecentMessage(loginSessionId,chatSessionId);
}

void DataCenter::resetRecentMessageList(const QString &chatSessionId, std::shared_ptr<bite_im::GetRecentMsgRsp> resp)
{
    // 拿到 chatSessionId 对应的消息列表，并清空
    // 注意此处务必是引用类型，才是修改哈希表内部的内容.
    QList<Message>& messageList = (*recentMessages)[chatSessionId];
    messageList.clear();

    // 遍历响应结果的列表
    for (auto& m : resp->msgList()) {
        Message message;
        message.load(m);
        messageList.push_back(message);
    }
}

chatSessionInfo *DataCenter::findSessionById(const QString &chatSessionId)
{
    // 为空直接返回空指针
    if(chatSessionList == nullptr)
    {
        return nullptr;
    }
    // 遍历会话列表查看是否有这个会话Id，有直接返回，没有再返回空指针
    for(auto & c : *chatSessionList)
    {
        if(c.chatSessionId == chatSessionId)
        {
            return &c;
        }
    }
    return nullptr;
}

chatSessionInfo *DataCenter::findChatSessionByUserId(const QString &userId)
{
    // 为空直接返回空指针
    if(chatSessionList == nullptr)
    {
        return nullptr;
    }
    // 遍历会话列表查看是否有这个会话Id，有直接返回，没有再返回空指针
    for(auto & c : *chatSessionList)
    {
        if(c.userId == userId)
        {
            return &c;
        }
    }
    return nullptr;
}
}

QString &DataCenter::getCurrentChatSessionId()
{
    return currentChatSessionId;
}

void DataCenter::setCurrentChatSessionId(const QString &chatSessionId)
{
    this->currentChatSessionId = chatSessionId;
}



// end namespace
}












