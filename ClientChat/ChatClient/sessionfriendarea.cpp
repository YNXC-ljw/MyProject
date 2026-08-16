#include "sessionfriendarea.h"

#include "debug.h"

#include <QScrollBar>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QStyleOption>
#include <QPainter>

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
        this->addItem(icon,"可怡"+ QString::number(i),"最后一条消息" + QString::number(i));
    }

#endif

}

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

void sessionFriendArea::addItem(const QIcon &avatar, const QString &name, const QString &text)
{
    SessionFriendItem* item = new SessionFriendItem(this, avatar, name, text);
    container->layout()->addWidget(item);
}

//////////////////////////////////////////////////////////////////
/// 会话 Item
//////////////////////////////////////////////////////////////////

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
    QLabel* messageLabel = new QLabel();
    messageLabel->setText(text);
    messageLabel->setFixedHeight(35);
    messageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // 头像处于 0, 0 位置，占据 2 行，占据 2 列
    layout->addWidget(avatarBtn, 0, 0, 2, 2);
    // 名字处于 0, 2 位置，占据 1 行，占据 1 列
    layout->addWidget(nameLabel, 0, 2, 1, 1);
    // 消息预览处于 1, 2 位置，占据 1 行，占据 1 列
    layout->addWidget(messageLabel, 1, 2, 1, 1);

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

}
















