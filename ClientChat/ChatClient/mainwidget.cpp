#include "mainwidget.h"
#include "ui_mainwidget.h"

#include "sessionfriendarea.h"
#include "debug.h"
#include "selfinfowidget.h"
#include "sessiondetailwidget.h"
#include "groupsessiondetailwidget.h"
#include "addfrienddialog.h"
#include "model/datacenter.h"

#include <QHBoxLayout>

using namespace model;

MainWidget* MainWidget::instance = nullptr;

MainWidget *MainWidget::getInstance()
{
    if(instance == nullptr)
    {
        // 此处不传入参数，表示以桌面为父窗口
        instance = new MainWidget();
    }
    return instance;
}

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    this->setGeometry(400,200,880,600);

    this->setWindowTitle(" ");
    this->setWindowIcon(QIcon(":/resource/image/logo.png"));

    // 初始化主窗口
    initMainWindow();
    // 初始化窗口左侧
    initLeftWindow();
    // 初始化窗口中间
    initMidWindow();
    // 初始化窗口右侧
    initRightWindow();
    // 初始化信号槽
    initSignalSlot();
}

MainWidget::~MainWidget()
{
    delete ui;
}
//////////////////////////////////////////////////////////////////////////////
/// 初始化操作
//////////////////////////////////////////////////////////////////////////////
void MainWidget::initMainWindow()
{
    QHBoxLayout* layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);   // 去掉边距，更干净
    layout->setSpacing(0);                    // 去掉控件间距
    this->setLayout(layout);

    windowLeft = new QWidget();
    windowMid = new QWidget();
    windowRight = new QWidget();

    windowLeft->setFixedWidth(70);
    windowMid->setFixedWidth(200);
    windowRight->setMinimumWidth(600);

    windowLeft->setStyleSheet("QWidget { background-color: rgb(224,224,224); }");
    windowMid->setStyleSheet("QWidget { background-color: rgb(238,238,240); }");
    windowRight->setStyleSheet("QWidget { background-color: rgb(250,250,250); }");

    layout->addWidget(windowLeft);
    layout->addWidget(windowMid);
    layout->addWidget(windowRight);

}

void MainWidget::initLeftWindow()
{
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setContentsMargins(0, 30, 0, 0);   // 去掉边距，更干净
    layout->setSpacing(20);                    // 去掉控件间距
    windowLeft->setLayout(layout);

    // 添加用户头像
    userAvatar = new QPushButton();
    userAvatar->setFixedSize(45,45);
    userAvatar->setIconSize(QSize(45,45));
    // userAvatar->setIcon(QIcon(":/resource/image/avatar.jpg"));
    userAvatar->setStyleSheet("QPushButton { border : none; background-color: transparent; }");
    layout->addWidget(userAvatar,1,Qt::AlignTop | Qt::AlignHCenter); // 置顶居中
    // 添加会话标签按钮
    sessionTabBtn = new QPushButton();
    sessionTabBtn->setFixedSize(45,45);
    sessionTabBtn->setIconSize(QSize(45,45));
    sessionTabBtn->setIcon(QIcon(":/resource/image/session_active.png"));
    sessionTabBtn->setStyleSheet("QPushButton { border : none; background-color: transparent; }");
    layout->addWidget(sessionTabBtn,1,Qt::AlignTop | Qt::AlignHCenter);
    // 添加朋友列表标签按钮
    friendTabBtn = new QPushButton();
    friendTabBtn->setFixedSize(45,45);
    friendTabBtn->setIconSize(QSize(45,45));
    friendTabBtn->setIcon(QIcon(":/resource/image/friend_inactive.png"));
    friendTabBtn->setStyleSheet("QPushButton { border : none; background-color: transparent; }");
    layout->addWidget(friendTabBtn,1,Qt::AlignTop | Qt::AlignHCenter);
    // 添加好友申请列表标签按钮
    applyTabBtn = new QPushButton();
    applyTabBtn->setFixedSize(45,45);
    applyTabBtn->setIconSize(QSize(45,45));
    applyTabBtn->setIcon(QIcon(":/resource/image/apply_inactive.png"));
    applyTabBtn->setStyleSheet("QPushButton { border : none; background-color: transparent; }");
    layout->addWidget(applyTabBtn,1,Qt::AlignTop | Qt::AlignHCenter);

    layout->addStretch(20); // 添加“弹簧”将控件往上挤
}

void MainWidget::initMidWindow()
{
    QGridLayout* layout = new QGridLayout();

    layout->setContentsMargins(0,20,0,0);
    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(10);
    windowMid->setLayout(layout);

    searchLine = new QLineEdit();
    searchLine->setFixedHeight(30);
    searchLine->setPlaceholderText("搜索");
    searchLine->setStyleSheet("QLineEdit { border-radius: 5px; background-color: rgb(250,250,250); padding-left: 5px;};");

    addFriendBtn = new QPushButton();
    addFriendBtn->setFixedSize(30,30);
    addFriendBtn->setIcon(QIcon(":/resource/image/add_friend.png"));
    QString style = "QPushButton { border-radius: 5px; background-color: rgb(230,230,232)};";
    style += "QPushButton:pressed {background-color: rgb(212,212,213)};";
    addFriendBtn->setStyleSheet(style);

    sessionFriArea = new sessionFriendArea();

    // 为了更灵活的控制边距，只影响搜索框按钮这一行，不影响下方列表这一行
    // 创建空白的 widget 填充到布局管理器中.
    QWidget* spacer1 = new QWidget();
    spacer1->setFixedWidth(10);
    QWidget* spacer2 = new QWidget();
    spacer2->setFixedWidth(10);
    QWidget* spacer3 = new QWidget();
    spacer3->setFixedWidth(10);

    layout->addWidget(spacer1, 0, 0);
    layout->addWidget(searchLine, 0, 1);
    layout->addWidget(spacer2, 0, 2);
    layout->addWidget(addFriendBtn, 0, 3);
    layout->addWidget(spacer3, 0, 4);
    layout->addWidget(sessionFriArea, 1, 0, 1, 5); // 后面两个参数表示该窗口横跨1行占5列

}

void MainWidget::initRightWindow()
{
    // 1.创建右侧窗口布局管理器
    QVBoxLayout* vlayout = new QVBoxLayout();
    vlayout->setContentsMargins(0,0,0,0);
    vlayout->setSpacing(0);
    vlayout->setAlignment(Qt::AlignTop);
    windowRight->setLayout(vlayout);

    // 2.创建上方标题栏
    QWidget *titleWidget = new QWidget();
    titleWidget->setFixedHeight(62);
    titleWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    //titleWidget->setObjectName();
    vlayout->addWidget(titleWidget);

    // 3.给标题栏添加布局管理器,并且加上按钮和标题
    QHBoxLayout* hlayout = new QHBoxLayout();
    hlayout->setContentsMargins(5,0,5,0);
    hlayout->setSpacing(0);
    titleWidget->setLayout(hlayout);
    // 标题
    sessionTitleLabel = new QLabel();
    sessionTitleLabel->setStyleSheet("QLabel { font-size: 22px;border-bottom: 1px solid rgb(230,230,230);}");
#if TEST_UI
    // 为了测试界面临时增加的. 实际这里的内容, 应该是使用从服务器获取的数据来设置.
    sessionTitleLabel->setText("你好");
#endif
    hlayout->addWidget(sessionTitleLabel);

    extraBtn = new QPushButton();
    extraBtn->setFixedSize(30, 30);
    extraBtn->setIconSize(QSize(30, 30));
    extraBtn->setIcon(QIcon(":/resource/image/more.png"));
    extraBtn->setStyleSheet("QPushButton { border:none; background-color: rgb(245, 245, 245); } QPushButton:pressed { background-color: rgb(220,220,220)}");
    hlayout->addWidget(extraBtn);

    // 4.添加消息展示区
    sessionShowArea = new SessionShowArea();
    vlayout->addWidget(sessionShowArea);

    // 5.添加消息编辑区
    messageEditArea = new MessageEditArea();
    vlayout->addWidget(messageEditArea,0,Qt::AlignBottom);

}

void MainWidget::initSignalSlot()
{
    model::DataCenter *dataCenter = model::DataCenter::getInstance();

    //////////////////////////////////////////
    /// 连接信号槽，处理标签页按钮切换问题
    //////////////////////////////////////////
    connect(sessionTabBtn,&QPushButton::clicked,this,&MainWidget::switchTabToSession);
    connect(friendTabBtn,&QPushButton::clicked,this,&MainWidget::switchTabToFriend);
    connect(applyTabBtn,&QPushButton::clicked,this,&MainWidget::switchTabToApply);

    //////////////////////////////////////////
    /// 连接信号槽，处理头像点击显示个人信息事件
    //////////////////////////////////////////
    connect(userAvatar,&QPushButton::clicked,this,[=](){
        SelfInfoWidget* selfInfoWidget = new SelfInfoWidget(this);
        selfInfoWidget->exec(); // 弹出模态对话框(影响整个窗口)
        // selfInfoWidget->show(); // 弹出非模态对话框
    });

    //////////////////////////////////////////
    /// 连接信号槽，处理消息展示区详情按钮点击事件
    //////////////////////////////////////////
    connect(extraBtn,&QPushButton::clicked,this,[=](){

#if GROUP_SESSION_DETAIL_WIDGET
        bool isSingleChat = false;
#else
        bool isSingleChat = true;
#endif
        if(isSingleChat){
            // 单聊窗口
            SessionDetailWidget* sessionDetailWidget = new SessionDetailWidget(this);
            sessionDetailWidget->exec();
        }else{
            // 群聊窗口
            GroupSessionDetailWidget* groupSessionDetialWidget = new GroupSessionDetailWidget(this);
            groupSessionDetialWidget->exec();
        }
    });

    //////////////////////////////////////////
    /// 连接信号槽，处理添加好友按钮信号
    //////////////////////////////////////////
    connect(addFriendBtn,&QPushButton::clicked,this,[=](){
        AddFriendDialog* addFriendDialog = new AddFriendDialog(this);
        addFriendDialog->exec();
    });

    //////////////////////////////////////////
    /// 修改搜索框内容，弹出添加好友对话框
    //////////////////////////////////////////
    connect(searchLine,&QLineEdit::textEdited,this,[=](){
        // 取出搜索框输入内容，设置到新弹框的输入框里面
        const QString& searchKey = searchLine->text();

        AddFriendDialog* addFriendDialog = new AddFriendDialog(this);
        addFriendDialog->setSearchKey(searchKey);
        // 清空主窗口中的文本
        searchLine->setText("");
        addFriendDialog->exec();
    });

    //////////////////////////////////////////
    /// 获取个人信息(跟数据打交道)
    //////////////////////////////////////////
    // 根据dataCenter获取到具体的网络数据
    connect(dataCenter,&model::DataCenter::getMyselfDone,this,[=](){
        // 从 DataCenter 中拿到响应结果的 myself，把头像取出来显示到界面上
        auto myself = dataCenter->getMyself();
        userAvatar->setIcon(myself->avatar);
    });
    dataCenter->getMyselfAsync();

    //////////////////////////////////////////
    /// 获取好友列表
    //////////////////////////////////////////
    loadFriendList();

    //////////////////////////////////////////
    /// 获取会话列表
    //////////////////////////////////////////
    loadSessionList();

    //////////////////////////////////////////
    /// 好友申请列表
    //////////////////////////////////////////
    loadApplyList();
}

/////////////////////////////////////////////////////////////////////////////////
/// 处理标签页按钮切换的槽函数
/////////////////////////////////////////////////////////////////////////////////
void MainWidget::switchTabToSession()
{
    // 1.标记当前处于哪个页面
    activeTab = SESSION_LIST;
    // 2.调整各标签图片
    sessionTabBtn->setIcon(QIcon(":/resource/image/session_active.png"));
    friendTabBtn->setIcon(QIcon(":/resource/image/friend_inactive.png"));
    applyTabBtn->setIcon(QIcon(":/resource/image/apply_inactive.png"));
    // 3.在主窗口中间部分加载出会话列表
    this->loadSessionList();
}

void MainWidget::switchTabToFriend()
{
    // 1.标记当前处于哪个页面
    activeTab = FRIEND_LIST;
    // 2.调整各标签图片
    sessionTabBtn->setIcon(QIcon(":/resource/image/session_inactive.png"));
    friendTabBtn->setIcon(QIcon(":/resource/image/friend_active.png"));
    applyTabBtn->setIcon(QIcon(":/resource/image/apply_inactive.png"));
    // 3.在主窗口中间部分加载出会话列表
    this->loadFriendList();

}

void MainWidget::switchTabToApply()
{
    // 1.标记当前处于哪个页面
    activeTab = APPLY_LIST;
    // 2.调整各标签图片
    sessionTabBtn->setIcon(QIcon(":/resource/image/session_inactive.png"));
    friendTabBtn->setIcon(QIcon(":/resource/image/friend_inactive.png"));
    applyTabBtn->setIcon(QIcon(":/resource/image/apply_active.png"));
    // 3.在主窗口中间部分加载出会话列表
    this->loadApplyList();

}

void MainWidget::switchToSession(const QString &userId)
{
    // 1.在会话列表中，找到对应的会话元素
    DataCenter* dataCenter = DataCenter::getInstance();
    dataCenter->findChatSessionByUserId(userId);
    // 2.把对应的会话置顶

    // 3.切换到会话列表标签页

    // 4.加载这个会话的历史消息
}
/////////////////////////////////////////////////////////////////////////////
/// 处理标签按钮点击后的界面显示
/////////////////////////////////////////////////////////////////////////////
// 加载会话列表
void MainWidget::loadSessionList()
{
    DataCenter* dataCenter = DataCenter::getInstance();
    if(dataCenter->getChatSessionList() == nullptr)
    {
        // 从服务器获取
        connect(dataCenter,&DataCenter::getChatSessionListDone,this,&MainWidget::updateChatSessionList,Qt::UniqueConnection);
        dataCenter->getChatSessionListAsync();
    }else{
        // 从内存中获取
        updateChatSessionList();
    }
}

void MainWidget::updateChatSessionList()
{
    // 如果当前页面不是会话列表界面，不渲染画面
    if(activeTab != SESSION_LIST)
    {
        return;
    }

    DataCenter* dataCenter = DataCenter::getInstance();
    QList<chatSessionInfo>* chatSessionList = dataCenter->getChatSessionList();

    // 清空原先已经存在的会话列表
    sessionFriArea->clear();

    for(const auto& c : *chatSessionList)
    {
        if (c.lastMessage.messageType == TEXT_TYPE) {
            sessionFriArea->addItem(SESSIONITEM_TYPE, c.chatSessionId, c.avatar, c.chatSessionName, c.lastMessage.content);
        } else if (c.lastMessage.messageType == IMAGE_TYPE) {
            sessionFriArea->addItem(SESSIONITEM_TYPE, c.chatSessionId, c.avatar, c.chatSessionName, "[图片]");
        } else if (c.lastMessage.messageType == FILE_TYPE) {
            sessionFriArea->addItem(SESSIONITEM_TYPE, c.chatSessionId, c.avatar, c.chatSessionName, "[文件]");
        } else if (c.lastMessage.messageType == SPEECH_TYPE) {
            sessionFriArea->addItem(SESSIONITEM_TYPE, c.chatSessionId, c.avatar, c.chatSessionName, "[语音]");
        } else {
            LOG() << "错误的消息类型! messageType=" << c.lastMessage.messageType;
        }

    }
}

// 加载好友列表
void MainWidget::loadFriendList()
{
    // 好友列表数据存储在 dataCenter 中。先判断其中是否有数据
    // 如果有就从本地加载；如果没有就从服务器中加载
    DataCenter* dataCenter = DataCenter::getInstance();
    if(dataCenter->getFriendList() == nullptr)
    {
        // 从服务器中拿
        connect(dataCenter,&DataCenter::getFriendListDone,this,&MainWidget::updateFriendList,Qt::UniqueConnection);// 只触发一次
        dataCenter->getFriendListAsync();
    }else{
        // 从内存中拿
        updateFriendList();
    }
}

void MainWidget::updateFriendList()
{
    // 如果当前标签页不是好友页，不渲染界面
    if(activeTab != FRIEND_LIST)
    {
        return;
    }

    DataCenter* dataCenter = DataCenter::getInstance();
    QList<UserInfo>* friendList = dataCenter->getFriendList();

    // 清空上一次打开客户端时残留的列表内容
    sessionFriArea->clear();

    // 添加friendList中的元素到好友列表界面
    for(const auto& f : *friendList)
    {
        sessionFriArea->addItem(FRIENDITEM_TYPE,f.userId,f.avatar,f.nickName,f.description);
    }
}

// 加载好友申请列表
void MainWidget::loadApplyList()
{
    DataCenter* dataCenter = DataCenter::getInstance();
    if (dataCenter->getApplyList() != nullptr) {
        // 本地有数据，直接加载
        updateApplyList();
    } else {
        // 本地没有数据，通过网络加载
        connect(dataCenter,&DataCenter::getApplyListDone,this,&MainWidget::updateApplyList,Qt::UniqueConnection);
        dataCenter->getApplyListAsync();
    }

}

void MainWidget::updateApplyList()
{
    if (activeTab != APPLY_LIST) {
        // 当前标签页不是 "好友申请列表"，界面上不做任何渲染
        return;
    }
    DataCenter* dataCenter = DataCenter::getInstance();
    QList<UserInfo>* applyList = dataCenter->getApplyList();

    sessionFriArea->clear();

    for (const auto& u : *applyList) {
        // 直接把这个 UserInfo 对象添加到界面上即可，不涉及到 PB 和 自己的结构之间的转换。
        // 此处 UserInfo 的 description 不需要填写进来。好友申请列表中，不显示用户的签名的（这个位置被替换成了两个按钮）
        sessionFriArea->addItem(APPLYITEM_TYPE, u.userId, u.avatar, u.nickName, "");
    }

}

void MainWidget::loadRecentMessage(const QString &chatSessionId)
{
    // 先判断 DataCenter 中是否存在数据，如果存在直接渲染到界面上；如果没有，就从网络中获取
    DataCenter* dataCenter = DataCenter::getInstance();
    if(dataCenter->getRecentMessageList(chatSessionId) != nullptr)
    {
        // 本地数据刷新界面
        updateRecentMessage(chatSessionId);
    }else{
        // 从服务器拿数据刷新界面
        connect(dataCenter,&DataCenter::getRecentMessageDone,this,&MainWidget::updateRecentMessage,Qt::UniqueConnection);
        dataCenter->getRecentMessageAsync(chatSessionId);
    }
}

void MainWidget::updateRecentMessage(const QString &chatSessionId)
{
    // 1.拿到对应会话的消息列表
    DataCenter* dataCenter = DataCenter::getInstance();
    auto *recentMessageList = dataCenter->getRecentMessageList(chatSessionId);

    // 2.清空原有界面上的消息
    sessionShowArea->clear();

    // 3.通过消息列表将数据展示到消息展示区(使用头插的方式将数据显示到界面上，让最近的消息展示在最底下)
    for(int i = recentMessageList->size() - 1;i >= 0;i--)
    {
        const Message& message = recentMessageList->at(i);
        // 两个用户Id不同说明就是左边消息
        bool isLeft = message.sender.userId != dataCenter->getMyself()->userId;
        sessionShowArea->addFrontMessageItem(isLeft,message);
    }

    // 4.设置会话标题
    chatSessionInfo* chatSessionInfo = dataCenter->findSessionById(chatSessionId);
    if(chatSessionInfo != nullptr)
    {
        // 把会话信息中的会话标题设置到对应位置
        sessionTitleLabel->setText(chatSessionInfo->chatSessionName);
    }

    // 5.保存当前选中的会话是哪一个
    dataCenter->setCurrentChatSessionId(chatSessionId);

    // 6.自动把滚动条滚动到末尾，显示最近一条消息界面
    sessionShowArea->scrollToEnd();
}















