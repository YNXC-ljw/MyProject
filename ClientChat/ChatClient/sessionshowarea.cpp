#include "sessionshowarea.h"

#include <QScrollBar>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFontMetrics>
#include <QPainter>
#include <QPainterPath>

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

void SessionShowArea::addMessageItem(bool isLeft, const Message &message)
{
    // 构造MessageItem,添加到布局管理器中
    MessageItem* messageItem = MessageItem::makeMessageItem(isLeft,message);
    container->layout()->addWidget(messageItem);
}

void SessionShowArea::addFrontMessageItem(bool isLeft, const Message &message)
{
    MessageItem* messageItem = MessageItem::makeMessageItem(isLeft,message);
    QVBoxLayout* layout = dynamic_cast<QVBoxLayout*>(container->layout());
    layout->insertWidget(0,messageItem);
}

void SessionShowArea::clear()
{
    // 遍历布局管理器清空消息元素

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
    QWidget* contentWidget = nullptr;
    switch (message.messageType) {
    case TEXT_TYPE:
        contentWidget = makeTextMessageItem(isLeft,message.content);
        break;
    case FILE_TYPE:
        contentWidget = makeFileMessageItem();
        break;
    case IMAGE_TYPE:
        contentWidget = makeImageMessageItem();
        break;
    case SPEECH_TYPE:
        contentWidget = makeSpeechMessageItem();
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

QWidget *MessageItem::makeTextMessageItem(bool isLeft, const QString &text)
{
    MessageContentLabel* messageContentLabel = new MessageContentLabel(text,isLeft);
    return messageContentLabel;
}

QWidget *MessageItem::makeFileMessageItem()
{
    return nullptr;

}

QWidget *MessageItem::makeImageMessageItem()
{
    return nullptr;

}

QWidget *MessageItem::makeSpeechMessageItem()
{
    return nullptr;

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

// 这个函数会在该空间被显示的时候自动调用到
void MessageContentLabel::paintEvent(QPaintEvent *event)
{
    (void) event;
    // 1.获取到父元素的宽度
    QObject* object = this->parent();
    if(!object->isWidgetType())
    {
        // 说明这个对象的父元素不是预期的 QWidget，此时不需要任何后续的绘制操作
        return;
    }
    QWidget* parent = dynamic_cast<QWidget*>(object);
    int width = parent->width(); //包含两侧边框

    // 2.计算当前文本，如果是一行放置，需要多宽
    QFontMetrics metrics(this->label->font());
    int totalWidth = metrics.horizontalAdvance(this->label->text()); // 文本总宽度

    // 3.计算行数(40表示每行带的左右边距)
    int rows = (totalWidth / (width - 40));
    if(rows == 1)
    {
        // 如果只有一行，那消息体宽度就是文字宽度 + 40边距
        width = totalWidth + 40;
    }

    // 4.根据行数计算出消息展示文本框的总高度(20表示上下边距)
    int height = rows * (this->label->font().pixelSize() * 1.2) + 20;

    // 5.绘制圆角矩形和箭头
    QPainter painter(this);
    QPainterPath path;
    // 设置 抗锯齿
    painter.setRenderHint(QPainter::Antialiasing);
    if(isLeft){
        painter.setPen(QPen(QColor(255,255,255))); //设置画笔和画刷
        painter.setBrush(QColor(255,255,255));

        // 绘制圆角矩形
        painter.drawRoundedRect(10,0,width,height,10,10);
        // 绘制箭头
        path.moveTo(10,15);
        path.lineTo(0,20);
        path.lineTo(10,25);
        path.closeSubpath(); // 绘制的线形成闭合的多边形，才能使用Brush进行颜色填充
        painter.drawPath(path); // 真正的绘制操作

        this->label->setGeometry(10,0,width,height);
    }else{
        painter.setPen(QPen(QColor(157, 242, 159)));
        painter.setBrush(QColor(157, 242, 159));

        // 圆角矩形左侧边的横坐标位置
        int leftPos = this->width() - width - 10; // 10 是用来容纳 箭头 的宽度
        // 圆角矩形右侧边的横坐标位置
        int rightPos = this->width() - 10;
        // 绘制圆角矩形
        painter.drawRoundedRect(leftPos, 0, width, height, 10, 10);
        // 绘制箭头
        path.moveTo(rightPos, 15);
        path.lineTo(rightPos + 10, 20);
        path.lineTo(rightPos, 25);
        path.closeSubpath();
        painter.drawPath(path);

        this->label->setGeometry(leftPos, 0, width, height);
    }

    // 6. 重新设置父元素的高度，确保父元素足够高，能够容纳下上述绘制的消息显示的区域
    // 注意高度要涵盖之前名字和时间的 label 的高度，以及留点冗余空间。
    parent->setFixedHeight(height + 50);

}


















