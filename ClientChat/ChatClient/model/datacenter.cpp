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

QList<UserInfo>* DataCenter::getFriendList()
{
    return friendList;
}

// end namespace
}












