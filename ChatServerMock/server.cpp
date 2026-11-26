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
bite_im::MessageInfo makeTextMessageInfo(int index,const QString& chatSessionId, const QByteArray& avatar)
{
    bite_im::MessageInfo messageInfo;
    messageInfo.setMessageId(QString::number(3000 + index));
    messageInfo.setChatSessionId(chatSessionId);
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
bite_im::MessageInfo makeImageMessageInfo(int index,const QString& chatSessionId,const QByteArray& avatar)
{
    bite_im::MessageInfo messageInfo;
    messageInfo.setMessageId(QString::number(3000 + index));
    messageInfo.setChatSessionId(chatSessionId);
    messageInfo.setTimestamp(getTime());
    messageInfo.setSender(makeUserInfo(index, avatar));

    bite_im::ImageMessageInfo imageMessageInfo;
    imageMessageInfo.setFileId("testImage");
    // 由于真实服务器推送本身不带图片内容，只有fileId
    // 获取图片内容只能通过fileId发送第二次请求，从getSingleFile
    // 中获取图片资源
    // imageMessageInfo.setImageContent();

    bite_im::MessageContent messageContent;
    messageContent.setMessageType(bite_im::MessageTypeGadget::MessageType::IMAGE);
    messageContent.setImageMessage(imageMessageInfo);
    messageInfo.setMessage(messageContent);
    return messageInfo;
}
bite_im::MessageInfo makeFileMessageInfo(int index,const QString& chatSessionId,const QByteArray& avatar)
{
    bite_im::MessageInfo messageInfo;
    messageInfo.setMessageId(QString::number(3000 + index));
    messageInfo.setChatSessionId(chatSessionId);
    messageInfo.setTimestamp(getTime());
    messageInfo.setSender(makeUserInfo(index, avatar));

    bite_im::FileMessageInfo fileMessageInfo;
    fileMessageInfo.setFileId("testFile");
    // 由于真实服务器推送本身不带图片内容，只有fileId
    // 获取图片内容只能通过fileId发送第二次请求，从getSingleFile
    // 中获取图片资源
    fileMessageInfo.setFileName("text.txt");
    fileMessageInfo.setFileSize(0);

    bite_im::MessageContent messageContent;
    messageContent.setMessageType(bite_im::MessageTypeGadget::MessageType::FILE);
    messageContent.setFileMessage(fileMessageInfo);
    messageInfo.setMessage(messageContent);
    return messageInfo;
}
bite_im::MessageInfo makeSpeechMessageInfo(int index,const QString& chatSessionId,const QByteArray& avatar)
{
    bite_im::MessageInfo messageInfo;
    messageInfo.setMessageId(QString::number(3000 + index));
    messageInfo.setChatSessionId(chatSessionId);
    messageInfo.setTimestamp(getTime());
    messageInfo.setSender(makeUserInfo(index, avatar));

    bite_im::SpeechMessageInfo speechMessageInfo;
    speechMessageInfo.setFileId("testSpeech");
    // 由于真实服务器推送本身不带图片内容，只有fileId
    // 获取图片内容只能通过fileId发送第二次请求，从getSingleFile


    bite_im::MessageContent messageContent;
    messageContent.setMessageType(bite_im::MessageTypeGadget::MessageType::SPEECH);
    messageContent.setSpeechMessage(speechMessageInfo);
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

    // 获取最近消息
    httpServer.route("/service/message_storage/get_recent",[=](const QHttpServerRequest& req){
        return this->getRecentMessageList(req);
    });

    // 发送消息
    httpServer.route("/service/message_transmit/new_message",[=](const QHttpServerRequest& req){
        return this->sendMessage(req);
    });

    // 处理修改用户昵称
    httpServer.route("/service/user/set_nickname",[=](const QHttpServerRequest& req){
        return this->setNickName(req);
    });

    // 处理修改用户签名
    httpServer.route("/service/user/set_description",[=](const QHttpServerRequest& req){
        return this->setDescription(req);
    });

    // 获取短信验证码
    httpServer.route("/service/user/get_phone_verify_code",[=](const QHttpServerRequest& req){
        return this->getVerifyCode(req);
    });

    // 修改电话号码
    httpServer.route("/service/user/set_phone",[=](const QHttpServerRequest& req){
        return this->changePhone(req);
    });

    // 修改用户头像
    httpServer.route("/service/user/set_avatar",[=](const QHttpServerRequest& req){
        return this->changeAvatar(req);
    });

    // 删除好友
    httpServer.route("/service/friend/remove_friend",[=](const QHttpServerRequest& req){
        return this->deleteFriend(req);
    });

    // 申请添加好友
    httpServer.route("/service/friend/add_friend_apply",[=](const QHttpServerRequest& req){
        return this->addFriendApply(req);
    });

    // 同意好友申请
    httpServer.route("/service/friend/add_friend_process",[=](const QHttpServerRequest& req){
        return this->acceptFriendApply(req);
    });

    // 创建群聊会话
    httpServer.route("/service/friend/create_chat_session",[=](const QHttpServerRequest& req){
        return this->createGroupSession(req);
    });

    // 获取会话群聊列表
    httpServer.route("/service/friend/get_chat_session_member",[=](const QHttpServerRequest& req){
        return this->groupSessionMemberList(req);
    });

    // 获取搜索结果
    httpServer.route("/service/friend/search_friend",[=](const QHttpServerRequest& req){
        return this->searchFriendResult(req);
    });

    // 搜索历史消息
    httpServer.route("/service/message_storage/search_history",[=](const QHttpServerRequest& req){
        return this->searchHistoryMessage(req);
    });
    // 搜索历史消息
    httpServer.route("/service/message_storage/search_history",[=](const QHttpServerRequest& req){
        return this->searchHistoryMessageByTime(req);
    });

    // 用户名登录
    httpServer.route("/service/user/username_login",[=](const QHttpServerRequest& req){
        return this->usernameLogin(req);
    });
    // 用户名注册
    httpServer.route("/service/user/username_register",[=](const QHttpServerRequest& req){
        return this->userRegister(req);
    });
    // 手机号登录
    httpServer.route("/service/user/phone_login",[=](const QHttpServerRequest& req){
        return this->phoneLogin(req);
    });
    // 手机号注册
    httpServer.route("/service/user/phone_register",[=](const QHttpServerRequest& req){
        return this->phoneRegister(req);
    });
    // 获取单个文件
    httpServer.route("/service/file/get_single_file",[=](const QHttpServerRequest& req){
        return this->getSingleFile(req);
    });
    // 语音转文字
    httpServer.route("/service/speech/recognition",[=](const QHttpServerRequest& req){
        return this->speechConvert(req);
    });
    return true;
}

QHttpServerResponse HttpServer::getUserInfo(const QHttpServerRequest& req)
{
    // 1.解析请求,把req里的body经过pb反序列化
    bite_im::GetUserInfoReq pbReq;
    pbReq.deserialize(&serializer,req.body());
    LOG() << "[REQ 获取用户信息] requestId = " << pbReq.requestId() << ", loginSessionId = " << pbReq.sessionId();

    // 2.将反序列化好的内容构造成响应数据
    bite_im::GetUserInfoRsp pbResp;
    pbResp.setRequestId(pbReq.requestId());
    pbResp.setSuccess(true);
    pbResp.setErrmsg("");

    bite_im::UserInfo userInfo;
    userInfo.setUserId("1029");
    userInfo.setNickname("张三");
    userInfo.setPhone("18246262332");
    userInfo.setDescription("这是一个个性签名");
    userInfo.setAvatar(readFiletoQByteArray(":/resource/image/avatar.jpg"));
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
    LOG() << "[REQ 获取好友列表] requestId = " << pbReq.requestId() << ", loginSessionId = " << pbReq.sessionId();

    // 2.将反序列化好的请求，构造成响应数据
    bite_im::GetFriendListRsp pbResp;
    pbResp.setRequestId(pbReq.requestId());
    pbResp.setSuccess(true);
    pbResp.setErrmsg("");

    // 3.构造假数据添加到相应数据中
    QByteArray avatar = readFiletoQByteArray(":/resource/image/groupAvatar.jpg");
    QList<bite_im::UserInfo> friendList;
    for(int i = 0;i < 20;i++)
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
    LOG() << "[REQ 获取会话列表] requestId = " << pbReq.requestId() << ", loginSessionId = " << pbReq.sessionId();

    // 2.构造响应数据
    bite_im::GetChatSessionListRsp pbResp;
    pbResp.setRequestId(pbReq.requestId());
    pbResp.setErrmsg("");
    pbResp.setSuccess(true);

    // 3.构造假单聊数据填充会话列表
    QByteArray avatar = readFiletoQByteArray(":/resource/image/groupAvatar.jpg");
    QList<bite_im::ChatSessionInfo> chatSessionList;
    for (int i = 0; i < 30; ++i) {
        bite_im::ChatSessionInfo chatSessionInfo;
        chatSessionInfo.setChatSessionId(QString::number(2000 + i));
        chatSessionInfo.setChatSessionName("会话" + QString::number(i));
        chatSessionInfo.setSingleChatFriendId(QString::number(1000 + i));
        chatSessionInfo.setAvatar(avatar);
        bite_im::MessageInfo messageInfo = makeTextMessageInfo(i, chatSessionInfo.chatSessionId(), avatar);

        chatSessionInfo.setPrevMessage(messageInfo);
        chatSessionList.push_back(chatSessionInfo);
    }
    // 构造一个群聊数据
    QByteArray groupAvatar = readFiletoQByteArray(":/resource/image/avatar.jpg");
    bite_im::ChatSessionInfo chatSessionInfo;
    chatSessionInfo.setChatSessionId(QString::number(2100));
    chatSessionInfo.setChatSessionName("会话" + QString::number(2000));
    chatSessionInfo.setSingleChatFriendId("");
    chatSessionInfo.setAvatar(groupAvatar);
    bite_im::MessageInfo messageInfo = makeTextMessageInfo(0, chatSessionInfo.chatSessionId(), avatar);
    chatSessionInfo.setPrevMessage(messageInfo);
    chatSessionList.push_back(chatSessionInfo);
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

QHttpServerResponse HttpServer::getRecentMessageList(const QHttpServerRequest &req)
{
    // 1.将客户端发起的请求反序列化
    bite_im::GetRecentMsgReq pbReq;
    pbReq.deserialize(&serializer,req.body());
    LOG() << "[REQ 获取最近消息] requestId = " << pbReq.requestId() << ", chatSessionId = " << pbReq.chatSessionId() << ",loginSessionId = " << pbReq.sessionId();

    // 2.构造假的响应数据
    bite_im::GetRecentMsgRsp pbResp;
    pbResp.setErrmsg("");
    pbResp.setRequestId(pbReq.requestId());
    pbResp.setSuccess(true);

    QByteArray avatar = readFiletoQByteArray(":/resource/image/groupAvatar.jpg");
    QList<bite_im::MessageInfo> messageList;
    for (int i = 0; i < 30; ++i) {
        // 文本消息
        bite_im::MessageInfo messageInfo = makeTextMessageInfo(i, "2000", avatar);
        messageList.push_back(messageInfo);
    }
    // 图片消息
    bite_im::MessageInfo imageMessageInfo = makeImageMessageInfo(30, "2000", avatar);
    messageList.push_back(imageMessageInfo);
    // 文件消息
    bite_im::MessageInfo fileMessageInfo = makeFileMessageInfo(31, "2000",avatar);
    messageList.push_back(fileMessageInfo);
    // 语音消息
    bite_im::MessageInfo speechMessageInfo = makeSpeechMessageInfo(32, "2000",avatar);
    messageList.push_back(speechMessageInfo);

    pbResp.setMsgList(messageList);
    // 序列化并构造HTTP响应
    QByteArray body = pbResp.serialize(&serializer);

    return QHttpServerResponse("application/x-protobuf",body);

}

QHttpServerResponse HttpServer::sendMessage(const QHttpServerRequest &req)
{
    // 解析请求
    bite_im::NewMessageReq pbReq;
    pbReq.deserialize(&serializer, req.body());
    LOG() << "[REQ 发送消息] requestId=" << pbReq.requestId() << ", loginSessionId=" << pbReq.sessionId()
          << ", chatSessionId=" << pbReq.chatSessionId() << ", messageType=" << pbReq.message().messageType();

    if (pbReq.message().messageType() == bite_im::MessageTypeGadget::MessageType::STRING) {
        LOG() << "发送的消息内容=" << pbReq.message().stringMessage().content();
    }

    // 构造响应
    bite_im::NewMessageRsp pbResp;
    pbResp.setRequestId(pbReq.requestId());
    pbResp.setSuccess(true);
    pbResp.setErrmsg("");

    QByteArray body = pbResp.serialize(&serializer);

    // 构造 HTTP 响应
    return QHttpServerResponse("application/x-protobuf",body);
}

QHttpServerResponse HttpServer::setNickName(const QHttpServerRequest &req)
{
    // 解析请求
    bite_im::SetUserNicknameReq pbReq;
    pbReq.deserialize(&serializer, req.body());
    LOG() << "[REQ 修改用户昵称] requestId=" << pbReq.requestId() << ", loginSessionId=" << pbReq.sessionId()
          << ", nickname=" << pbReq.nickname();

    // 构造响应
    bite_im::SetUserNicknameRsp pbResp;
    pbResp.setRequestId(pbReq.requestId());
    pbResp.setSuccess(true);
    pbResp.setErrmsg("");
    QByteArray body = pbResp.serialize(&serializer);

    return QHttpServerResponse("application/x-protobuf",body);
}

QHttpServerResponse HttpServer::setDescription(const QHttpServerRequest &req)
{
    // 解析请求
    bite_im::SetUserDescriptionReq pbReq;
    pbReq.deserialize(&serializer, req.body());
    LOG() << "[REQ 修改用户签名] requestId=" << pbReq.requestId() << ", loginSessionId=" << pbReq.sessionId()
          << ", desc=" << pbReq.description();

    // 构造响应
    bite_im::SetUserDescriptionRsp pbResp;
    pbResp.setRequestId(pbReq.requestId());
    pbResp.setSuccess(true);
    pbResp.setErrmsg("");
    QByteArray body = pbResp.serialize(&serializer);

    return QHttpServerResponse("application/x-protobuf",body);
}

QHttpServerResponse HttpServer::getVerifyCode(const QHttpServerRequest &req)
{
    // 解析请求
    bite_im::PhoneVerifyCodeReq pbReq;
    pbReq.deserialize(&serializer, req.body());
    LOG() << "[REQ 获取短信验证码] requestId=" << pbReq.requestId() << ", phone=" << pbReq.phoneNumber();

    // 构造响应 body
    bite_im::PhoneVerifyCodeRsp pbResp;
    pbResp.setRequestId(pbReq.requestId());
    pbResp.setSuccess(true);
    pbResp.setErrmsg("");
    pbResp.setVerifyCodeId("testVerifyCodeId");
    QByteArray body = pbResp.serialize(&serializer);

    return QHttpServerResponse("application/x-protobuf",body);
}

QHttpServerResponse HttpServer::changePhone(const QHttpServerRequest &req)
{
    // 解析请求
    bite_im::SetUserPhoneNumberReq pbReq;
    pbReq.deserialize(&serializer, req.body());
    LOG() << "[REQ 修改手机号] requestId=" << pbReq.requestId() << ", loginSessionId=" << pbReq.sessionId() << ", phone=" << pbReq.phoneNumber()
          << ", verifyCodeId=" << pbReq.phoneVerifyCodeId() << ", verifyCode=" << pbReq.phoneVerifyCode();

    // 构造响应 body
    bite_im::SetUserPhoneNumberRsp pbResp;
    pbResp.setRequestId(pbReq.requestId());
    pbResp.setSuccess(true);
    pbResp.setErrmsg("");
    QByteArray body = pbResp.serialize(&serializer);

    return QHttpServerResponse("application/x-protobuf",body);
}

QHttpServerResponse HttpServer::changeAvatar(const QHttpServerRequest &req)
{
    // 解析请求
    bite_im::SetUserAvatarReq pbReq;
    pbReq.deserialize(&serializer, req.body());
    LOG() << "[REQ 修改头像] requestId=" << pbReq.requestId() << ", loginSessionId=" << pbReq.sessionId();

    // 构造响应 body
    bite_im::SetUserAvatarRsp pbResp;
    pbResp.setRequestId(pbReq.requestId());
    pbResp.setSuccess(true);
    pbResp.setErrmsg("");
    QByteArray body = pbResp.serialize(&serializer);

    return QHttpServerResponse("applocation/x-protobuf",body);
}

QHttpServerResponse HttpServer::deleteFriend(const QHttpServerRequest &req)
{
    // 解析请求
    bite_im::FriendRemoveReq pbReq;
    pbReq.deserialize(&serializer, req.body());
    LOG() << "[REQ 删除好友] requestId=" << pbReq.requestId() << ", loginSessionId=" << pbReq.sessionId()
          << ", peerId=" << pbReq.peerId();

    // 构造响应 body
    bite_im::FriendRemoveRsp pbResp;
    pbResp.setRequestId(pbReq.requestId());
    pbResp.setSuccess(true);
    pbResp.setErrmsg("");
    QByteArray body = pbResp.serialize(&serializer);

    return QHttpServerResponse("application/x-protobuf",body);
}

QHttpServerResponse HttpServer::addFriendApply(const QHttpServerRequest &req)
{
    // 解析请求
    bite_im::FriendAddReq pbReq;
    pbReq.deserialize(&serializer, req.body());
    LOG() << "[REQ 添加好友申请] requestId=" << pbReq.requestId() << ", loginSessionId=" << pbReq.sessionId()
          << ", userId=" << pbReq.respondentId();

    // 构造响应 body
    bite_im::FriendAddRsp pbResp;
    pbResp.setRequestId(pbReq.requestId());
    pbResp.setSuccess(true);
    pbResp.setErrmsg("");
    pbResp.setNotifyEventId("");
    QByteArray body = pbResp.serialize(&serializer);

    return QHttpServerResponse("application/x-protobuf",body);
}

QHttpServerResponse HttpServer::acceptFriendApply(const QHttpServerRequest &req)
{
    // 解析请求
    bite_im::FriendAddProcessReq pbReq;
    pbReq.deserialize(&serializer, req.body());
    LOG() << "[REQ 添加好友申请处理] requestId=" << pbReq.requestId() << ", loginSessionId=" << pbReq.sessionId()
          << ", applyUserId=" << pbReq.applyUserId() << ", agree=" << pbReq.agree();

    // 构造响应 body
    bite_im::FriendAddProcessRsp pbResp;
    pbResp.setRequestId(pbReq.requestId());
    pbResp.setSuccess(true);
    pbResp.setErrmsg("");
    pbResp.setNewSessionId("");
    QByteArray body = pbResp.serialize(&serializer);

    return QHttpServerResponse("application/x-protobuf",body);
}

QHttpServerResponse HttpServer::createGroupSession(const QHttpServerRequest &req)
{
    // 解析请求
    bite_im::ChatSessionCreateReq pbReq;
    pbReq.deserialize(&serializer, req.body());
    LOG() << "[REQ 创建会话] requestId=" << pbReq.requestId() << ", loginSessionId=" << pbReq.sessionId()
          << ", userIdList=" << pbReq.memberIdList();

    // 构造响应 body
    bite_im::ChatSessionCreateRsp pbResp;
    pbResp.setRequestId(pbReq.requestId());
    pbResp.setSuccess(true);
    pbResp.setErrmsg("");
    QByteArray body = pbResp.serialize(&serializer);

    return QHttpServerResponse("application/x-protobuf",body);
}

QHttpServerResponse HttpServer::groupSessionMemberList(const QHttpServerRequest &req)
{
    // 解析请求
    bite_im::GetChatSessionMemberReq pbReq;
    pbReq.deserialize(&serializer, req.body());
    LOG() << "[REQ 获取会话成员列表] requestId=" << pbReq.requestId() << ", loginSessionId=" << pbReq.sessionId()
          << ", chatSessionId=" << pbReq.chatSessionId();

    // 构造响应
    bite_im::GetChatSessionMemberRsp pbResp;
    pbResp.setRequestId(pbReq.requestId());
    pbResp.setSuccess(true);
    pbResp.setErrmsg("");

    // 循环的构造多个 userInfo, 添加到 memberInfoList 中
    QByteArray avatar = readFiletoQByteArray(":/resource/image/avatar.jpg");
    QList<bite_im::UserInfo> memberList;
    for (int i = 0; i < 10; ++i) {
        bite_im::UserInfo userInfo = makeUserInfo(i, avatar);
        memberList.push_back(userInfo);
    }
    pbResp.setMemberInfoList(memberList);

    // 序列化
    QByteArray body = pbResp.serialize(&serializer);

    return QHttpServerResponse("application/x-protobuf",body);
}

QHttpServerResponse HttpServer::searchFriendResult(const QHttpServerRequest &req)
{
    // 解析请求
    bite_im::FriendSearchReq pbReq;
    pbReq.deserialize(&serializer, req.body());
    LOG() << "[REQ 搜索好友] requestId=" << pbReq.requestId() << ", loginSessionId=" << pbReq.sessionId()
          << ", searchKey=" << pbReq.searchKey();

    // 构造响应 body
    bite_im::FriendSearchRsp pbResp;
    pbResp.setRequestId(pbReq.requestId());
    pbResp.setSuccess(true);
    pbResp.setErrmsg("");

    QByteArray avatar = readFiletoQByteArray(":/resource/image/avatar.jpg");
    QList<bite_im::UserInfo> userInfoList;
    for (int i = 0; i < 30; ++i) {
        bite_im::UserInfo userInfo = makeUserInfo(i, avatar);
        userInfoList.push_back(userInfo);
    }
    pbResp.setUserInfo(userInfoList);

    QByteArray body = pbResp.serialize(&serializer);

    return QHttpServerResponse("application/x-protobuf",body);
}

QHttpServerResponse HttpServer::searchHistoryMessage(const QHttpServerRequest &req)
{
    // 解析请求
    bite_im::MsgSearchReq pbReq;
    pbReq.deserialize(&serializer, req.body());
    LOG() << "[REQ 搜索历史消息] requestId=" << pbReq.requestId() << ", loginSessionId=" << pbReq.sessionId()
          << ", chatSessionId=" << pbReq.chatSessionId() << ", searchKey=" << pbReq.searchKey();

    // 构造响应 body
    bite_im::MsgSearchRsp pbResp;
    pbResp.setRequestId(pbReq.requestId());
    pbResp.setSuccess(true);
    pbResp.setErrmsg("");

    QByteArray avatar = readFiletoQByteArray(":/resource/image/avatar.jpg");
    QList<bite_im::MessageInfo> messageList;
    for (int i = 0; i < 10; ++i) {
        // 文本消息
        bite_im::MessageInfo message = makeTextMessageInfo(i, pbReq.chatSessionId(), avatar);
        messageList.push_back(message);
    }
    // 构造图片消息
    bite_im::MessageInfo imageMessage = makeImageMessageInfo(10, pbReq.chatSessionId(), avatar);
    messageList.push_back(imageMessage);
    // 构造文件消息
    bite_im::MessageInfo fileMessage = makeFileMessageInfo(11, pbReq.chatSessionId(), avatar);
    messageList.push_back(fileMessage);
    // 构造语音消息
    bite_im::MessageInfo speechMessage = makeSpeechMessageInfo(12, pbReq.chatSessionId(), avatar);
    messageList.push_back(speechMessage);

    pbResp.setMsgList(messageList);
    QByteArray body = pbResp.serialize(&serializer);

    return QHttpServerResponse("application/x-protobuf",body);
}

QHttpServerResponse HttpServer::searchHistoryMessageByTime(const QHttpServerRequest &req)
{
    // 解析请求
    bite_im::GetHistoryMsgReq pbReq;
    pbReq.deserialize(&serializer, req.body());
    LOG() << "[REQ 按时间搜索历史消息] requestId=" << pbReq.requestId() << ", loginSessionId=" << pbReq.sessionId()
          << ", chatSessionId=" << pbReq.chatSessionId() << ", begTime=" << pbReq.startTime() << ", endTime=" << pbReq.overTime();

    // 构造响应
    bite_im::GetHistoryMsgRsp pbResp;
    pbResp.setRequestId(pbReq.requestId());
    pbResp.setSuccess(true);
    pbResp.setErrmsg("");

    QByteArray avatar = readFiletoQByteArray(":/resource/image/defaultAvatar.png");
    QList<bite_im::MessageInfo> messageList;
    for (int i = 0; i < 10; ++i) {
        bite_im::MessageInfo message = makeTextMessageInfo(i, pbReq.chatSessionId(), avatar);
        messageList.push_back(message);
    }
    pbResp.setMsgList(messageList);
    QByteArray body = pbResp.serialize(&serializer);

    return QHttpServerResponse("application/x-protobuf",body);
}

QHttpServerResponse HttpServer::usernameLogin(const QHttpServerRequest &req)
{
    // 解析请求
    bite_im::UserLoginReq pbReq;
    pbReq.deserialize(&serializer, req.body());
    LOG() << "[REQ 用户名密码登录] requestId=" << pbReq.requestId() << ", username=" << pbReq.nickname()
          << ", password=" << pbReq.password();

    // 构造响应 body
    bite_im::UserLoginRsp pbResp;
    pbResp.setRequestId(pbReq.requestId());
    pbResp.setSuccess(true);
    pbResp.setErrmsg("");
    pbResp.setLoginSessionId("testLoginSessionId");
    QByteArray body = pbResp.serialize(&serializer);

    return QHttpServerResponse("application/x-protobuf",body);
}

QHttpServerResponse HttpServer::userRegister(const QHttpServerRequest &req)
{
    // 解析请求
    bite_im::UserRegisterReq pbReq;
    pbReq.deserialize(&serializer, req.body());
    LOG() << "[REQ 用户名密码注册] requestId=" << pbReq.requestId() << ", username=" << pbReq.nickname()
          << ", password=" << pbReq.password();

    // 构造响应 body
    bite_im::UserRegisterRsp pbResp;
    pbResp.setRequestId(pbReq.requestId());
    pbResp.setSuccess(true);
    pbResp.setErrmsg("");
    QByteArray body = pbResp.serialize(&serializer);

    return QHttpServerResponse("application/x-protobuf",body);
}

QHttpServerResponse HttpServer::phoneLogin(const QHttpServerRequest &req)
{
    // 解析请求
    bite_im::PhoneLoginReq pbReq;
    pbReq.deserialize(&serializer, req.body());
    LOG() << "[REQ 手机号登录] requestId=" << pbReq.requestId() << ", phone=" << pbReq.phoneNumber()
          << ", verifyCodeId=" << pbReq.verifyCodeId() << ", verifyCode=" << pbReq.verifyCode();

    // 构造响应
    bite_im::PhoneLoginRsp pbResp;
    pbResp.setRequestId(pbReq.requestId());
    pbResp.setSuccess(true);
    pbResp.setErrmsg("");
    pbResp.setLoginSessionId("testLoginSessionId");
    QByteArray body = pbResp.serialize(&serializer);

    return QHttpServerResponse("application/x-protobuf",body);
}

QHttpServerResponse HttpServer::phoneRegister(const QHttpServerRequest &req)
{
    // 解析请求
    bite_im::PhoneRegisterReq pbReq;
    pbReq.deserialize(&serializer,req.body());
    LOG() << "[REQ 手机号注册] requestId = " << pbReq.requestId() << ", phone = " << pbReq.phoneNumber()
          << ", verifyCodeId = " << pbReq.verifyCodeId() << ", verifyCode = " << pbReq.verifyCode();

    // 构造响应
    bite_im::PhoneRegisterRsp pbResp;
    pbResp.setErrmsg("");
    pbResp.setRequestId(pbReq.requestId());
    pbResp.setSuccess(true);
    QByteArray body = pbResp.serialize(&serializer);

    return QHttpServerResponse("application/x-protobuf",body);
}

QHttpServerResponse HttpServer::getSingleFile(const QHttpServerRequest &req)
{
    // 解析请求
    bite_im::GetSingleFileReq pbReq;
    pbReq.deserialize(&serializer,req.body());
    LOG() << "[REQ 获取单个文件] requestId = " << pbReq.requestId() << ", fileId = " << pbReq.fileId();

    // 构造响应
    bite_im::GetSingleFileRsp pbResp;
    pbResp.setErrmsg("");
    pbResp.setRequestId(pbReq.requestId());
    pbResp.setSuccess(true);

    bite_im::FileDownloadData fileDownloadData;
    fileDownloadData.setFileId(pbReq.fileId());
    // 此处后续要能够支持三个情况，图片文件，普通文件，语音文件．
    // 直接使用 fileId 做区分
    if (pbReq.fileId() == "testImage") {
        fileDownloadData.setFileContent(readFiletoQByteArray(":/resource/image/avatar.jpg"));
    } else if (pbReq.fileId() == "testFile") {
        fileDownloadData.setFileContent(readFiletoQByteArray(":/resource/file/test.txt"));
    } else if (pbReq.fileId() == "testSpeech") {
        // 由于此处暂时还没有音频文件．得后面写了 录音功能 才能生成．
        fileDownloadData.setFileContent(readFiletoQByteArray(":/resource/file/speech.pcm"));
    } else {
        pbResp.setSuccess(false);
        pbResp.setErrmsg("fileId 不是预期的测试 fileId");
    }
    pbResp.setFileData(fileDownloadData);

    QByteArray body = pbResp.serialize(&serializer);

    return QHttpServerResponse("application/x-protobuf",body);
}

QHttpServerResponse HttpServer::speechConvert(const QHttpServerRequest &req)
{
    // 解析请求 body
    bite_im::SpeechRecognitionReq pbReq;
    pbReq.deserialize(&serializer, req.body());
    LOG() << "[REQ 语音转文字] requestId=" << pbReq.requestId() << ", loginSessionId=" << pbReq.sessionId();

    // 构造响应 body
    bite_im::SpeechRecognitionRsp pbResp;
    pbResp.setRequestId(pbReq.requestId());
    pbResp.setSuccess(true);
    pbResp.setErrmsg("");
    pbResp.setRecognitionResult("你好你好，这是一段语音消息，你好你好，这是一段语音消息");
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
            disconnect(this,&WebSocketServer::sendTextResp,this,nullptr);
            disconnect(this,&WebSocketServer::deleteFriendResp,this,nullptr);
            disconnect(this,&WebSocketServer::addFriendApply,this,nullptr);
            disconnect(this,&WebSocketServer::acceptFriendApply,this,nullptr);
            disconnect(this,&WebSocketServer::sendCreateSession,this,nullptr);
            disconnect(this,&WebSocketServer::sendImageMessage,this,nullptr);
            disconnect(this,&WebSocketServer::sendFileMessage,this,nullptr);
            disconnect(this,&WebSocketServer::sendSpeechMessage,this,nullptr);
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

        connect(this,&WebSocketServer::sendTextResp,this,[=](){
            // 此处就可以捕获到socket对象
            if(socket == nullptr || !socket->isValid())
            {
                LOG() << "socket是一个无效对象！";
            }

            QByteArray avatar = readFiletoQByteArray(":/resource/image/groupAvatar.jpg");
            bite_im::MessageInfo messageInfo = makeTextMessageInfo(this->messageIndex++, "2000", avatar);

            bite_im::NotifyNewMessage notifyNewMessage;
            notifyNewMessage.setMessageInfo(messageInfo);

            bite_im::NotifyMessage notifyMessage;
            notifyMessage.setNotifyEventId("");
            notifyMessage.setNotifyType(bite_im::NotifyTypeGadget::NotifyType::CHAT_MESSAGE_NOTIFY);
            notifyMessage.setNewMessageInfo(notifyNewMessage);

            // 序列化
            QByteArray body = notifyMessage.serialize(&this->serializer);

            // 发送消息给客户端
            socket->sendBinaryMessage(body);

            LOG() << "发送文本消息响应: " << messageInfo.message().stringMessage().content();
        });
        connect(this,&WebSocketServer::sendImageMessage,this,[=](){
            // 此处就可以捕获到socket对象
            if(socket == nullptr || !socket->isValid())
            {
                LOG() << "socket是一个无效对象！";
            }

            QByteArray avatar = readFiletoQByteArray(":/resource/image/groupAvatar.jpg");
            bite_im::MessageInfo messageInfo = makeImageMessageInfo(this->messageIndex++, "2000", avatar);

            bite_im::NotifyNewMessage notifyNewMessage;
            notifyNewMessage.setMessageInfo(messageInfo);

            bite_im::NotifyMessage notifyMessage;
            notifyMessage.setNotifyEventId("");
            notifyMessage.setNotifyType(bite_im::NotifyTypeGadget::NotifyType::CHAT_MESSAGE_NOTIFY);
            notifyMessage.setNewMessageInfo(notifyNewMessage);

            // 序列化
            QByteArray body = notifyMessage.serialize(&this->serializer);

            // 发送消息给客户端
            socket->sendBinaryMessage(body);

            LOG() << "发送文本消息响应: ";
        });
        connect(this,&WebSocketServer::sendFileMessage,this,[=](){
            // 此处就可以捕获到socket对象
            if(socket == nullptr || !socket->isValid())
            {
                LOG() << "socket是一个无效对象！";
            }

            QByteArray avatar = readFiletoQByteArray(":/resource/image/groupAvatar.jpg");
            bite_im::MessageInfo messageInfo = makeFileMessageInfo(this->messageIndex++, "2000", avatar);

            bite_im::NotifyNewMessage notifyNewMessage;
            notifyNewMessage.setMessageInfo(messageInfo);

            bite_im::NotifyMessage notifyMessage;
            notifyMessage.setNotifyEventId("");
            notifyMessage.setNotifyType(bite_im::NotifyTypeGadget::NotifyType::CHAT_MESSAGE_NOTIFY);
            notifyMessage.setNewMessageInfo(notifyNewMessage);

            // 序列化
            QByteArray body = notifyMessage.serialize(&this->serializer);

            // 发送消息给客户端
            socket->sendBinaryMessage(body);

            LOG() << "发送文件消息响应: ";
        });
        connect(this,&WebSocketServer::sendSpeechMessage,this,[=](){
            // 此处就可以捕获到socket对象
            if(socket == nullptr || !socket->isValid())
            {
                LOG() << "socket是一个无效对象！";
            }

            QByteArray avatar = readFiletoQByteArray(":/resource/image/groupAvatar.jpg");
            bite_im::MessageInfo messageInfo = makeSpeechMessageInfo(this->messageIndex++, "2000", avatar);

            bite_im::NotifyNewMessage notifyNewMessage;
            notifyNewMessage.setMessageInfo(messageInfo);

            bite_im::NotifyMessage notifyMessage;
            notifyMessage.setNotifyEventId("");
            notifyMessage.setNotifyType(bite_im::NotifyTypeGadget::NotifyType::CHAT_MESSAGE_NOTIFY);
            notifyMessage.setNewMessageInfo(notifyNewMessage);

            // 序列化
            QByteArray body = notifyMessage.serialize(&this->serializer);

            // 发送消息给客户端
            socket->sendBinaryMessage(body);

            LOG() << "发送文件消息响应: ";
        });
        connect(this,&WebSocketServer::deleteFriendResp,this,[=](){
            if(socket == nullptr || !socket->isValid())
            {
                LOG() << "socket是一个无效对象!";
            }

            bite_im::NotifyMessage notifyMessage;
            notifyMessage.setNotifyEventId("");
            notifyMessage.setNotifyType(bite_im::NotifyTypeGadget::NotifyType::FRIEND_REMOVE_NOTIFY);

            bite_im::NotifyFriendRemove notifyFriendRemove;
            notifyFriendRemove.setUserId("1000");
            notifyMessage.setFriendRemove(notifyFriendRemove);

            // 序列化
            QByteArray body = notifyMessage.serialize(&serializer);

            // 将消息发送给客户端
            socket->sendBinaryMessage(body);
            // 打印日志
            LOG() << "发送删除好友响应 userId = " << notifyFriendRemove.userId();
        });
        connect(this,&WebSocketServer::addFriendApply,this,[=](){
            if (socket == nullptr || !socket->isValid()) {
                LOG() << "socket 对象无效";
                return;
            }

            bite_im::NotifyMessage notifyMessage;
            notifyMessage.setNotifyEventId("");
            notifyMessage.setNotifyType(bite_im::NotifyTypeGadget::NotifyType::FRIEND_ADD_APPLY_NOTIFY);

            QByteArray avatar = readFiletoQByteArray(":/resource/image/avatar.jpg");
            bite_im::UserInfo userInfo = makeUserInfo(100, avatar);

            bite_im::NotifyFriendAddApply friendAddApply;
            friendAddApply.setUserInfo(userInfo);

            notifyMessage.setFriendAddApply(friendAddApply);

            QByteArray body = notifyMessage.serialize(&serializer);
            socket->sendBinaryMessage(body);
            LOG() << "通知对方好友申请数据";
        });
        connect(this,&WebSocketServer::acceptFriendApply,this,[=](bool agree){
            if (socket == nullptr || !socket->isValid()) {
                LOG() << "socket 对象无效";
                return;
            }

            bite_im::NotifyMessage notifyMessage;
            notifyMessage.setNotifyEventId("");
            notifyMessage.setNotifyType(bite_im::NotifyTypeGadget::NotifyType::FRIEND_ADD_PROCESS_NOTIFY);

            QByteArray avatar = readFiletoQByteArray(":/resource/image/defaultAvatar.png");
            bite_im::UserInfo userInfo = makeUserInfo(100, avatar);

            bite_im::NotifyFriendAddProcess friendAddProcess;
            friendAddProcess.setUserInfo(userInfo);
            friendAddProcess.setAgree(agree);

            notifyMessage.setFriendProcessResult(friendAddProcess);

            QByteArray body = notifyMessage.serialize(&serializer);
            socket->sendBinaryMessage(body);
            LOG() << "通知好友申请的处理结果 userId=" << userInfo.userId() << ", agree=" << agree;

        });
        connect(this,&WebSocketServer::sendCreateSession,this,[=](){
            if (socket == nullptr || !socket->isValid()) {
                LOG() << "socket 对象无效";
                return;
            }

            QByteArray avatar = readFiletoQByteArray(":/resource/image/groupAvatar.png");

            bite_im::NotifyMessage notifyMessage;
            notifyMessage.setNotifyEventId("");
            notifyMessage.setNotifyType(bite_im::NotifyTypeGadget::NotifyType::CHAT_SESSION_CREATE_NOTIFY);
            bite_im::MessageInfo messageInfo = makeTextMessageInfo(0, "2100", avatar);

            bite_im::ChatSessionInfo chatSessionInfo;
            chatSessionInfo.setChatSessionId("2100");
            chatSessionInfo.setSingleChatFriendId("");
            chatSessionInfo.setChatSessionName("新的群聊");
            chatSessionInfo.setPrevMessage(messageInfo);
            chatSessionInfo.setAvatar(avatar);

            bite_im::NotifyNewChatSession newChatSession;
            newChatSession.setChatSessionInfo(chatSessionInfo);
            notifyMessage.setNewChatSessionInfo(newChatSession);

            // 序列化操作
            QByteArray body = notifyMessage.serialize(&serializer);

            // 通过 websocket 推送数据
            socket->sendBinaryMessage(body);
            LOG() << "通知创建会话!";

        });

    });

    // 2.绑定端口,启动服务
    bool ok = webSocketServer.listen(QHostAddress::Any,8001);
    return ok;
}













