#include "sessionfriendarea.h"

#include "debug.h"
#include "mainwidget.h"
#include "model/datacenter.h"
#include "model/data.h"

#include <QScrollBar>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QStyleOption>
#include <QPainter>

using namespace model;

sessionFriendArea::sessionFriendArea(QWidget *parent)
    : QScrollArea{parent}
{
    // 1.设置必要的属性
    // 设置了该属性才能开启滚动效果
    this->setWidgetResizable(true);
    // 设置滚动条样式
    this->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{width:7px;background:transparent;}"
        "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:none;}"
        "QScrollBar::sub-line:vertical,QScrollBar::add-line:vertical{height:0px;background:none;}"
        "QScrollBar::handle:vertical{background-color:rgb(130,130,130);border-radius:5px;min-height:22px;}"
        "QScrollBar::handle:vertical:hover{background-color:rgb(90,90,90);}");
    this->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal { height: 0px; };"); // 水平滚动条不显示，设为0

    // 2.创建widget，设置到滚动区内
    container = new QWidget();
    container->setFixedWidth(200);
    this->setWidget(container); // 将widget和QScrollArea重合，以实现滚动条效果

    // 3.给widget设置布局管理器
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    layout->setAlignment(Qt::AlignTop); // 所有消息列表向上顶
    container->setLayout(layout);

    // // 4. 简单测试一下滚动的效果
    // for (int i = 0; i < 100; ++i) {
    //     QPushButton* btn = new QPushButton();
    //     btn->setText("按钮");
    //     layout->addWidget(btn);
    // }

    // 构造一些临时数据，用来作为“界面调试”的依据。后续再删除掉
#if TEST_UI
    QIcon icon(":/resource/image/avatar.jpg");
    for(int i = 0;i < 30;i++)
    {
        this->addItem(SESSIONITEM_TYPE,QString::number(i),icon,"张三"+ QString::number(i),"最后一条消息" + QString::number(i));
    }

#endif

}

// 清除列表中的元素
void sessionFriendArea::clear()
{
    QLayout* layout = container->layout();
    // 遍历布局管理器中的所有元素，并依次从布局管理器中删除掉
    for (int i = layout->count() - 1; i >= 0; --i) {
        // takeAt 就能移除对应下标的元素
        QLayoutItem* item = layout->takeAt(i);
        // 别忘了，还需要对这个对象进行 "释放"
        if (item->widget()) {
            // 把这个移除的内容的 widget 进行释放.
            // 正常使用的时候，new 出来的对象添加到布局管理器的....
            delete item->widget();
        }
    }
}

void sessionFriendArea::addItem(ItemType itemType,const QString& id,const QIcon &avatar, const QString &name, const QString &text)
{
    SessionFriendItem* item = nullptr;
    if (itemType == SESSIONITEM_TYPE) {
        item = new SessionItem(this, id, avatar, name, text);
    } else if (itemType == FRIENDITEM_TYPE) {
        item = new FriendItem(this, id, avatar, name, text);
    } else if (itemType == APPLYITEM_TYPE) {
        item = new ApplyItem(this, id, avatar, name);
    } else {
        LOG() << "错误的 ItemType! itemType=" << itemType;
        return;
    }
    container->layout()->addWidget(item);

}

void sessionFriendArea::clickItem(int index)
{
    if (index < 0 || index >= container->layout()->count()) {
        LOG() << "点击元素的下标超出范围！ index=" << index;
        return;
    }
    QLayoutItem* layoutItem = container->layout()->itemAt(index);
    if (layoutItem == nullptr || layoutItem->widget() == nullptr) {
        LOG() << "指定的元素不存在！ index=" << index;
        return;
    }
    SessionFriendItem* item = dynamic_cast<SessionFriendItem*>(layoutItem->widget());
    item->select();
}

//////////////////////////////////////////////////////////////////////////
/// 滚动区域会话 Item的实现
//////////////////////////////////////////////////////////////////////////

SessionFriendItem::SessionFriendItem(QWidget *owner, const QIcon &avatar, const QString &name, const QString &text)
    :owner(owner)
{
    this->setFixedHeight(70);
    this->setStyleSheet("QWidget { background-color: rgb(238,238,240)};");
    
    // 创建网格布局管理器
    QGridLayout* layout = new QGridLayout();
    layout->setContentsMargins(10,0,0,0);
    layout->setHorizontalSpacing(10);
    layout->setVerticalSpacing(0);
    this->setLayout(layout);
    
    // 创建头像
    QPushButton* avatarBtn = new QPushButton();
    avatarBtn->setFixedSize(50, 50);
    avatarBtn->setIconSize(QSize(50, 50));
    avatarBtn->setIcon(avatar);
    avatarBtn->setStyleSheet("QPushButton {border: none;}");
    avatarBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed); // 表示不论窗口变大变小，头像按钮大小都不变

    // 创建名字
    QLabel* nameLabel = new QLabel();
    nameLabel->setText(name);
    nameLabel->setStyleSheet("QLabel { font-size: 15px; font-weight: 600; }");
    nameLabel->setFixedHeight(35);
    nameLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // 创建消息预览的 label
    messageLabel = new QLabel();
    messageLabel->setText(text);
    messageLabel->setFixedHeight(35);
    messageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // 头像处于 0, 0 位置，占据 2 行，占据 2 列
    layout->addWidget(avatarBtn, 0, 0, 2, 2);
    // 名字处于 0, 2 位置，占据 1 行，占据 1 列
    layout->addWidget(nameLabel, 0, 2, 1, 8);
    // 消息预览处于 1, 2 位置，占据 1 行，占据 1 列
    layout->addWidget(messageLabel, 1, 2, 1, 8);

}

void SessionFriendItem::paintEvent(QPaintEvent *event)
{
    (void)event;
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

// 鼠标点击时触发该函数，点击修改背景颜色
void SessionFriendItem::mousePressEvent(QMouseEvent *event)
{
    (void) event;
    select();
}

void SessionFriendItem::enterEvent(QEnterEvent *event)
{
    (void) event;
    // 如果是被选中状态，直接返回
    if(this->selected)
    {
        return;
    }
    // 设置背景色
    this->setStyleSheet("QWidget { background-color: rgb(225, 225, 227);}");
}

void SessionFriendItem::leaveEvent(QEvent *event)
{
    (void) event;
    // 如果是被选中状态，直接返回
    if(this->selected)
    {
        return;
    }
    // 还原背景色
    this->setStyleSheet("QWidget { background-color: rgb(238,238,240)};");
}

void SessionFriendItem::select()
{
    // 取到所有Item，将所有点击过的样式清空，再将当前点击的Item样式修改为指定样式
    const QObjectList children = this->parentWidget()->children(); // 先拿到该元素的父元素，再通过该父元素找到所有子元素
    for(QObject* child : children)
    {
        if(!child->isWidgetType())
        {
            // 判定是否是一个widget
            continue;
        }
        // 确定是widget，就把child强转成sessionFriendItem
        SessionFriendItem* item = dynamic_cast<SessionFriendItem*>(child);
        if(item->selected)
        {
            item->selected = false;
            item->setStyleSheet("QWidget { background-color: rgb(238,238,240)};");
        }
    }
    // 将当前被点击的item背景设置为选中状态
    this->setStyleSheet("QWidget { background-color: rgb(21, 172, 112);}");
    this->selected = true;

    // 调用 active
    this->active();
}

void SessionFriendItem::active()
{

}

/////////////////////////////////////////
/// 会话Item的实现
/////////////////////////////////////////

SessionItem::SessionItem(QWidget *owner, const QString &chatSessionId, const QIcon &avatar, const QString &name, const QString &lastMessage)
    :SessionFriendItem(owner,avatar,name,lastMessage),chatSessionId(chatSessionId)
{
    DataCenter *dataCenter = DataCenter::getInstance();
    connect(dataCenter,&DataCenter::updateLastMessage,this,&SessionItem::updateLastMessage);
}

void SessionItem::active()
{
    // 点击之后要加载会话的历史消息列表
    LOG() << "点击SessionItem触发的逻辑！chatSessionId=" << chatSessionId;

    // 加载会话历史消息，既会涉及到界面渲染，又会涉及到内存数据操作、还会涉及到网络通信(所以直接在主窗口中进行代码管理)
    MainWidget* mainWidget = MainWidget::getInstance();
    mainWidget->loadRecentMessage(chatSessionId);
}
void SessionItem::updateLastMessage(const QString &chatSessionId)
{
    DataCenter* dataCenter = DataCenter::getInstance();
    if(this->chatSessionId != chatSessionId)
    {
        // 每个会话Item在程序启动时都绑定这个信号槽，但是只有会话Id匹配的才会更新最后一条消息
        return;
    }

    QList<Message>* messageList = dataCenter->getRecentMessageList(chatSessionId);
    if (messageList == nullptr || messageList->size() == 0) {
        // 当前会话没有任何消息，无需更新
        return;
    }

    const Message& lastMessage = messageList->back();

    // 3. 明确显示的文本内容
    // 由于消息有四种类型.
    // 文本消息，直接显示消息的内容；图片消息，直接显示 "[图片]"；文件消息，直接显示 "[文件]"；语音消息，直接显示 "[语音]"
    QString text;
    if (lastMessage.messageType == TEXT_TYPE) {
        text = lastMessage.content;
    } else if (lastMessage.messageType == IMAGE_TYPE) {
        text = "[图片]";
    } else if (lastMessage.messageType == FILE_TYPE) {
        text = "[文件]";
    } else if (lastMessage.messageType == SPEECH_TYPE) {
        text = "[语音]";
    } else {
        LOG() << "错误的消息类型!";
        return;
    }

    // 4.把内容显示到界面上
    this->messageLabel->setText(text);
}

/// /////////////////////////////////////
/// 好友Item的实现
/////////////////////////////////////////

FriendItem::FriendItem(QWidget *owner, const QString &userId, const QIcon &avatar,
                       const QString &name, const QString &description)
    :SessionFriendItem(owner,avatar,name,description),userId(userId)
{

}

void FriendItem::active()
{
    // TODO 点击后，要激活对应的会话列表元素
    LOG() << "点击FriendItem触发的逻辑！userId=" << userId;

    MainWidget* mainWidget = MainWidget::getInstance();
    mainWidget->switchToSession(userId);
}

/////////////////////////////////////////
/// 申请列表Item的实现
/////////////////////////////////////////

ApplyItem::ApplyItem(QWidget *owner, const QString &userId, const QIcon &avatar, const QString &name)
    :SessionFriendItem(owner,avatar,name,""),userId(userId)
{
    // 1.移除父类的 messageLabel
    QGridLayout* layout = dynamic_cast<QGridLayout*>(this->layout()); // 这里拿到的布局管理器是最初布局管理器的父类，需要强转
    layout->removeWidget(messageLabel);
    // 要记得释放内存避免资源泄露
    delete messageLabel;

    // 2.添加同意/拒绝按钮
    QPushButton* acceptBtn = new QPushButton();
    acceptBtn->setText("同意");
    QPushButton* rejectBtn = new QPushButton();
    rejectBtn->setText("拒绝");
    // 3.添加到布局管理器中
    layout->addWidget(acceptBtn,1,2,1,1);
    layout->addWidget(rejectBtn,1,3,1,1);

}

void ApplyItem::active()
{
    // 这个函数本身不需要实现任何内容
    LOG() << "点击ApplyItem触发的逻辑！userId=" << userId;
}






