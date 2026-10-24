#include "historymessagewidget.h"

#include <QGridLayout>
#include <QRadioButton>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QDateTimeEdit>
#include <QScrollArea>
#include <QScrollBar>

#include "debug.h"

using namespace model;

///////////////////////////////////////////////////////////
/// 表示一个历史消息元素
///////////////////////////////////////////////////////////

// 工厂函数就扮演着构造函数的角色
HistoryItem *HistoryItem::makeHistoryItem(const Message &message)
{
    // 1.创建历史消息对象
    HistoryItem* item = new HistoryItem();
    item->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

    // 2.创建布局管理器
    QGridLayout* layout = new QGridLayout();
    layout->setVerticalSpacing(0);
    layout->setHorizontalSpacing(10);
    layout->setContentsMargins(0,0,0,0);
    item->setLayout(layout);

    // 3.创建头像
    QPushButton* avatarBtn = new QPushButton();
    avatarBtn->setFixedSize(40,40);
    avatarBtn->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    avatarBtn->setIconSize(QSize(40,40));
    // 设置当前消息发送者的头像
    avatarBtn->setIcon(message.sender.avatar);
    avatarBtn->setStyleSheet("QPushButton { border: none; }");

    // 4.创建昵称和时间
    QLabel* name = new QLabel();
    name->setText(message.sender.nickName + "|" + message.time);
    name->setFixedHeight(20); // 高度设置为头像高度的一半
    name->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    name->setStyleSheet("QLabel { font-size: 12px; }");

    // 5.消息内容部分
    QWidget* contentWidget = nullptr;
    if(message.messageType == TEXT_TYPE)
    {
        // 文本消息
        QLabel* label = new QLabel();
        label->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        label->setWordWrap(true); // 自动换行
        label->setText(QString(message.content));
        label->adjustSize(); // 设置label自动调整大小
        contentWidget = label;
    }else if(message.messageType == SPEECH_TYPE){
        // 语音消息
    }else if(message.messageType == IMAGE_TYPE){
        // 图片消息
    }else if(message.messageType == FILE_TYPE){
        // 文件消息
    }else{
        LOG() << "错误的消息类型!messageType = " << message.messageType;
    }

    // 6.将上述内容添加到布局管理器中
    layout->addWidget(avatarBtn,0,0,2,1);
    layout->addWidget(name,0,1,1,1);
    layout->addWidget(contentWidget,1,1,5,1);

    return item;
}

///////////////////////////////////////////////////////////
/// 表示整个历史消息搜索框
///////////////////////////////////////////////////////////
HistoryMessageWidget::HistoryMessageWidget(QWidget* parent) :QDialog(parent)
{
    // 1.设置窗口基本属性
    this->setFixedSize(600,600);
    this->setWindowTitle("历史消息");
    this->setWindowIcon(QIcon(":/resource/image/logo.png"));
    this->setStyleSheet("QWidget { background-color: rgb(255,255,255);}");
    this->setAttribute(Qt::WA_DeleteOnClose);

    // 2.创建布局管理器
    QGridLayout* layout = new QGridLayout();
    layout->setSpacing(10);
    layout->setContentsMargins(30,30,30,0);
    this->setLayout(layout);

    // 3.创建单选按钮
    QRadioButton* keyRadioBtn = new QRadioButton();
    QRadioButton* timeRadioBtn = new QRadioButton();
    keyRadioBtn->setText("按关键词搜索");
    timeRadioBtn->setText("按时间搜索");
    keyRadioBtn->setStyleSheet("QRadioButton::indicator:checked { background-color: rgb(231,231,231);border-radius: 8px; }");
    timeRadioBtn->setStyleSheet("QRadioButton::indicator:checked { background-color: rgb(231,231,231);border-radius: 8px; }");
    // 默认按关键词查询
    keyRadioBtn->setChecked(true);
    layout->addWidget(keyRadioBtn,0,0,1,2);
    layout->addWidget(timeRadioBtn,0,2,1,2);

    // 4.创建搜索框
    QLineEdit* searchEdit = new QLineEdit();
    searchEdit->setFixedHeight(50);
    searchEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    searchEdit->setPlaceholderText("请输入搜索的关键词");
    searchEdit->setStyleSheet("QLineEdit { border:none; background-color: rgb(240,240,240); font-size: 16px; border-radius: 10px;padding-left: 10px;}");
    layout->addWidget(searchEdit,1,0,1,8);

    // 5.创建搜索按钮
    QPushButton* searchBtn = new QPushButton();
    searchBtn->setFixedSize(50,50);
    searchBtn->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    searchBtn->setIconSize(QSize(30,30));
    searchBtn->setIcon(QIcon(":/resource/image/search.png"));
    QString style = "QPushButton { border:none; border-radius: 5px; background-color: rgb(240,240,240); }";
    style += "QPushButton:pressed { backgorund-color: rgb(220,220,220);}";
    searchBtn->setStyleSheet(style);
    layout->addWidget(searchBtn,1,8,1,1);

    // 6．创建时间相关的部分控件，初始情况下要隐藏
    QLabel* begTag = new QLabel();
    begTag->setText("开始时间");
    QLabel* endTag = new QLabel();
    endTag->setText("结束时间");
    QDateTimeEdit* begTimeEdit = new QDateTimeEdit();
    QDateTimeEdit* endTimeEdit = new QDateTimeEdit();
    begTimeEdit->setFixedHeight(50);
    endTimeEdit->setFixedHeight(50);
    begTag->hide();
    endTag->hide();
    begTimeEdit->hide();
    endTimeEdit->hide();

    // 7.创建滚动区域
    initScrollArea(layout);

    // 8. 设置槽函数
    connect(keyRadioBtn, &QRadioButton::clicked, this, [=]() {
        // 把时间相关的控件，隐藏起来
        layout->removeWidget(begTag);
        layout->removeWidget(begTimeEdit);
        layout->removeWidget(endTag);
        layout->removeWidget(endTimeEdit);
        begTag->hide();
        begTimeEdit->hide();
        endTag->hide();
        endTimeEdit->hide();

        // 把关键词搜索框显示加入布局
        layout->addWidget(searchEdit, 1, 0, 1, 8);
        searchEdit->show();
    });

    connect(timeRadioBtn, &QRadioButton::clicked, this, [=]() {
        // 关键词搜索框隐藏并从布局中删除掉.
        layout->removeWidget(searchEdit);
        searchEdit->hide();

        // 把时间相关的控件，添加到布局中，并且进行显示.
        layout->addWidget(begTag, 1, 0, 1, 1);
        layout->addWidget(begTimeEdit, 1, 1, 1, 3);
        layout->addWidget(endTag, 1, 4, 1, 1);
        layout->addWidget(endTimeEdit, 1, 5, 1, 3);
        begTag->show();
        begTimeEdit->show();
        endTag->show();
        endTimeEdit->show();
    });

    // 构造临时数据
#if TEST_UI
    for(int i = 0;i < 30;i++)
    {
        // 注意此处代码和前面的差别.
        // 前面有个代码，UserInfo 必须要 new 出来才能构造. 当时 Item 对象里，持有了 const UserInfo& , 不是 new 的话
        // 就可能使引用指向的对象失效的.
        // 此处后续的代码，都是按照传值的方式来使用 message 的内容，不 new 也行.
        UserInfo sender;
        sender.userId = "";
        sender.nickName = "张三" + QString::number(i);
        sender.avatar = QIcon(":/resource/image/avatar.jpg");
        sender.description = "";
        sender.phone = "18612345678";
        Message message = Message::makeMessage(TEXT_TYPE, "", sender, QString("消息内容" + QString::number(i)).toUtf8(), "");
        this->addHistoryMessage(message);
    }
#endif


}

void HistoryMessageWidget::addHistoryMessage(const Message &message)
{
    HistoryItem* historyMessage = HistoryItem::makeHistoryItem(message);
    container->layout()->addWidget(historyMessage);
}

void HistoryMessageWidget::clear()
{
    QVBoxLayout* layout = dynamic_cast<QVBoxLayout*>(container->layout());
    for(int i = layout->count() - 1;i >= 0;i--)
    {
        // 这个写法和之前的takeAt是一样的
        QWidget* widget = layout->itemAt(i)->widget();
        if(widget == nullptr)
        {
            continue;
        }
        // 不是空直接删除
        layout->removeWidget(widget);
        widget->deleteLater();
    }
}

void HistoryMessageWidget::initScrollArea(QGridLayout* layout)
{
    // 1.创建滚动区域
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    scrollArea->setWidgetResizable(true);
    scrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width: 2px; background-color: rgb(255,255,255);}");
    scrollArea->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal { height:0; }");
    scrollArea->setStyleSheet("QScrollArea { border: none; }");

    // 2.创建 QWidget 区域放元素
    container = new QWidget();
    scrollArea->setWidget(container);

    // 3.创建 QWidget 内部的布局管理器
    QVBoxLayout* vlayout = new QVBoxLayout();
    vlayout->setContentsMargins(0,0,0,0);
    vlayout->setSpacing(10);
    vlayout->setAlignment(Qt::AlignTop);
    container->setLayout(vlayout);

    // 4.将滚动区域设置到网格布局管理器中
    layout->addWidget(scrollArea,2,0,1,9);
}












