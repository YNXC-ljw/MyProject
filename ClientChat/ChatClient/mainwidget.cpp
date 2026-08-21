#include "mainwidget.h"
#include "ui_mainwidget.h"

#include "sessionfriendarea.h"
#include "debug.h"


#include <QHBoxLayout>

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
    userAvatar->setIcon(QIcon(":/resource/image/avatar.jpg"));
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

    sessionFriendArea* sessionFriArea = new sessionFriendArea();

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

    QLabel* sessionTitleLabel = new QLabel();
    sessionTitleLabel->setStyleSheet("QLabel { font-size: 22px;border-bottom: 1px solid rgb(230,230,230);}");
#if TEST_UI
    // 为了测试界面临时增加的. 实际这里的内容, 应该是使用从服务器获取的数据来设置.
    sessionTitleLabel->setText("你好");
#endif
    hlayout->addWidget(sessionTitleLabel);

    QPushButton* extraBtn = new QPushButton();
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
    //////////////////////////////////////////
    /// 连接信号槽，处理标签页按钮切换问题
    //////////////////////////////////////////
    connect(sessionTabBtn,&QPushButton::clicked,this,&MainWidget::switchTabToSession);
    connect(friendTabBtn,&QPushButton::clicked,this,&MainWidget::switchTabToFriend);
    connect(applyTabBtn,&QPushButton::clicked,this,&MainWidget::switchTabToApply);
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
/////////////////////////////////////////////////////////////////////////////
/// 处理标签按钮点击后的界面显示
/////////////////////////////////////////////////////////////////////////////
void MainWidget::loadSessionList()
{

}

void MainWidget::loadFriendList()
{

}

void MainWidget::loadApplyList()
{

}

