#include "sessionshowarea.h"

#include <QScrollBar>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

using namespace model;

////////////////////////////////////////////////////
/// 表示消息展示区
////////////////////////////////////////////////////

SessionShowArea::SessionShowArea() {
    // 1.设置基本属性
    this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    this->setWidgetResizable(true);
    // 设置滚动条样式
    this->verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width: 2px; background-color: rgb(240,240,240);}");
    this->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal { height: 0;}");
    this->setStyleSheet("QScrollArea { border: none; }");


    // 2.创建widget，作为包含内部元素的容器
    container = new QWidget();
    this->setWidget(container);

    // 3.给container内部创建布局管理器
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    container->setLayout(layout);
}

////////////////////////////////////////////////////
/// 表示一个消息元素
/// 这个里面要能同时支持 文本消息 语音消息 图片消息 文件消息
/// 暂时只考虑文本消息，后续完善其他消息
////////////////////////////////////////////////////

MessageItem::MessageItem(bool isLeft)
    :isLeft(isLeft)
{

}

MessageItem *MessageItem::makeMessageItem(bool isLeft, const Message &message)
{
    // 1.创建对象和布局管理器
    MessageItem* messageItem = new MessageItem(isLeft);
    QGridLayout* layout = new QGridLayout();
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    // 这个messageItem最低不能低于100
    messageItem->setMinimumHeight(100);
    messageItem->setLayout(layout);

    // 2.创建头像
    QPushButton* avatarBtn = new QPushButton();
    avatarBtn->setFixedSize(40,40);
    avatarBtn->setIconSize(QSize(40,40));
    avatarBtn->setIcon(message.sender.avatar);
    if (isLeft) {
        layout->addWidget(avatarBtn, 0, 0, 2, 1, Qt::AlignTop | Qt::AlignLeft);
    } else {
        layout->addWidget(avatarBtn, 0, 1, 2, 1, Qt::AlignTop | Qt::AlignLeft);
    }

    // 3.创建名字和时间
    QLabel* nameLabel = new QLabel();
    nameLabel->setText(message.sender.nickName + " | " + message.time);
    nameLabel->setAlignment(Qt::AlignBottom);
    nameLabel->setStyleSheet("QLabel { font-size: 12px; color: rgb(178,178,178);}");
    if(isLeft)
    {
        layout->addWidget(nameLabel,0,1);
    }
    else
    {
        layout->addWidget(nameLabel,0,0,Qt::AlignRight);
    }

    // 4.创建消息体
    QWidget* contentWidget = new QWidget();
    switch (message.messageType) {
    case TEXT_TYPE:
        contentWidget = makeTextMessage();
        break;
    case FILE_TYPE:
        contentWidget = makeFileMessage();
        break;
    case IMAGE_TYPE:
        contentWidget = makeImageMessage();
        break;
    case SPEECH_TYPE:
        contentWidget = makeSpeechMessage();
        break;
    default:
        LOG() << "错误的消息类型！MessageType = " << message.messageType;
        break;
    }
    if(isLeft){
        layout->addWidget(contentWidget,1,1);
    }
    else{
        layout->addWidget(contentWidget,1,0);
    }

    return messageItem;
}

////////////////////////////////////////////////////
/// 创建类表示“文本消息”正文部分
////////////////////////////////////////////////////
MessageContentLabel::MessageContentLabel(const QString &text, bool isLeft)
    :isLeft(isLeft)
{

    QFont font;
    font.setFamily("微软雅黑");
    font.setPixelSize(16);

    this->label = new QLabel(this);
    this->label->setText(text);
    this->label->setFont(font);
    this->label->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    this->label->setWordWrap(true); // 文本自动换行
    this->setStyleSheet("QLabel { padding: 0 10px; line-height: 1.2;}");
}










