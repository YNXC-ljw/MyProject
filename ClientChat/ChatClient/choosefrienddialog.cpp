#include "choosefrienddialog.h"

#include <QHBoxLayout>
#include <QScrollArea>
#include <QScrollBar>
#include <QPushButton>
#include <QPainter>

#include "debug.h"

/////////////////////////////////////////////
/// 选择好友窗口中的一个 元素/好友项
/////////////////////////////////////////////

ChooseFriendItem::ChooseFriendItem(ChooseFriendDialog* owner,const QString& userId,const QIcon &avatar, const QString &name, bool checked)
    :userId(userId)
{
    // 1. 设置控件的基本属性
    this->setFixedHeight(50);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // 2. 设置布局管理器
    QHBoxLayout* layout = new QHBoxLayout();
    layout->setSpacing(10);
    layout->setContentsMargins(20, 0, 20, 0);
    this->setLayout(layout);

    // 3. 创建复选框
    checkBox = new QCheckBox();
    checkBox->setChecked(checked);
    checkBox->setFixedSize(25, 25);
    QString style = "QCheckBox::indicator { width: 20px; height: 20px; image: url(:/resource/image/unchecked.png);";
    style += "QCheckBox::indicator:checked { image: url(:/resource/image/checked.png); }";
    checkBox->setStyleSheet(style);

    // 4. 创建头像
    avatarBtn = new QPushButton();
    avatarBtn->setFixedSize(40, 40);
    avatarBtn->setIconSize(QSize(40, 40));
    avatarBtn->setIcon(avatar);

    // 5. 创建名字
    nameLabel = new QLabel();
    nameLabel->setText(name);

    // 6. 添加上述内容到布局管理器中
    layout->addWidget(checkBox);
    layout->addWidget(avatarBtn);
    layout->addWidget(nameLabel);

    // 7.连接信号槽
    connect(checkBox,&QCheckBox::toggled,this,[=](bool checked){
        if(checked){
            // 勾选了复选框，把Item添加到已选列表当中
            owner->addSelectedFriend(userId,avatar,name);
        }else{
            // 取消勾选
            owner->deleteSelectedFriend(userId);
        }
    });
}

void ChooseFriendItem::paintEvent(QPaintEvent *event)
{
    (void) event;
    // 根据鼠标进入的状态来绘制不同的颜色
    QPainter painter(this);
    if(isHover)
    {
        // 绘制成深色
        painter.fillRect(this->rect(),QColor(230,230,230));
    }else{
        // 绘制成浅色
        painter.fillRect(this->rect(),QColor(255,255,255));
    }
}

void ChooseFriendItem::enterEvent(QEnterEvent *event)
{
    (void) event;
    isHover = true;
    // 这里的update就相当于更新界面
    this->update();
    // 下列代码也是一样的
    //this->repaint();
}

void ChooseFriendItem::leaveEvent(QEvent *event)
{
    (void) event;
    isHover = false;
    this->update();
}

/////////////////////////////////////////////
/// 选择好友的窗口
/////////////////////////////////////////////

ChooseFriendDialog::ChooseFriendDialog(QWidget* parent) :QDialog(parent)
{
    // 1.设置窗口基本属性
    this->setWindowTitle("选择好友");
    this->setFixedSize(750,500);
    this->setWindowIcon(QIcon(":/resource/image/logo.png"));
    this->setStyleSheet("QDialog { background-color: rgb(255,255,255)}; ");
    this->setAttribute(Qt::WA_DeleteOnClose); // 关闭窗口时调用析构释放资源

    // 2.创建布局管理器
    QHBoxLayout* layout = new QHBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    this->setLayout(layout);

    // 3.设置左窗口
    initLeft(layout);

    // 4.设置右窗口
    initRight(layout);
}

    void ChooseFriendDialog::initLeft(QHBoxLayout *layout)
{
    // 1. 创建滚动区域
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    scrollArea->setWidgetResizable(true);
    scrollArea->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal { height: 0px;}");
    scrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width: 2px; background-color: rgb(255, 255, 255) }");
    scrollArea->setStyleSheet("QScrollArea { border: none; }");
    layout->addWidget(scrollArea,1);

    // 2. 创建 QWidget 设置到滚动区域中.
    totalContainer = new QWidget();
    totalContainer->setObjectName("totalContainer");
    totalContainer->setStyleSheet("#totalContainer { background-color: rgb(255, 255, 255); }");
    scrollArea->setWidget(totalContainer);
    scrollArea->setWidgetResizable(true);

    // 3. 创建左侧子窗口内部的 垂直布局管理器
    QVBoxLayout* vlayout = new QVBoxLayout();
    vlayout->setSpacing(0);
    vlayout->setContentsMargins(0, 0, 0, 0);
    vlayout->setAlignment(Qt::AlignTop);
    totalContainer->setLayout(vlayout);

// 还需要进一步的添加 vlayout 内部的元素, 才能看到效果!
// 此处也是先构造测试数据, 后续接入服务器之后, 从服务器拿到真实的好友列表, 再添加真实的数据
#if TEST_UI
    QIcon defaultAvatar(":/resource/image/avatar.jpg");
    for (int i = 0; i < 30; ++i) {
        this->addFriendItem(QString("1000") + QString::number(i),defaultAvatar, "张三" + QString::number(i), false);
    }
#endif

}

void ChooseFriendDialog::initRight(QHBoxLayout *layout)
{
    // 1. 创建右侧的布局管理器
    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->setContentsMargins(20, 0, 20, 20);
    gridLayout->setSpacing(10);
    layout->addLayout(gridLayout,1);

    // 2. 创建 "提示" label
    QLabel* tipLabel = new QLabel();
    tipLabel->setText("选择联系人");
    tipLabel->setFixedHeight(30);
    tipLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    tipLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    tipLabel->setStyleSheet("QLabel { font-size: 16px; font-weight: 700; }");

    // 3. 创建滚动区域
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width: 2px; background-color: rgb(255, 255, 255);}");
    scrollArea->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal {height: 0px;}");
    scrollArea->setStyleSheet("QScrollArea { border: none;}");
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // 4. 创建滚动区域中的 QWidget
    selectedContainer = new QWidget();
    selectedContainer->setObjectName("selectedContainer");
    selectedContainer->setStyleSheet("#selectedContainer { background-color: rgb(255, 255, 255); }");
    scrollArea->setWidget(selectedContainer);

    // 5. 创建 selectedContainer 中的 "垂直布局"
    QVBoxLayout* vlayout = new QVBoxLayout();
    vlayout->setSpacing(0);
    vlayout->setContentsMargins(0, 0, 0, 0);
    vlayout->setAlignment(Qt::AlignTop);
    selectedContainer->setLayout(vlayout);

    // 6.创建底部按钮
    QString style = "QPushButton { color: rgb(7,191,96); background-color: rgb(240,240,240); border-radius: 5px}";
    style += "QPushButton:hover { background-color: rgb(220,220,220); } QPushButton:pressed { background-color: rgb(200,200,200);}";

    QPushButton* okBtn = new QPushButton();
    okBtn->setFixedHeight(40);
    okBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    okBtn->setText("完成");
    okBtn->setStyleSheet(style);

    QPushButton* cancelBtn = new QPushButton();
    cancelBtn->setFixedHeight(40);
    cancelBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    cancelBtn->setText("取消");
    cancelBtn->setStyleSheet(style);

    // 7. 把上述控件添加到布局中
    gridLayout->addWidget(tipLabel, 0, 0, 1, 9);
    gridLayout->addWidget(scrollArea, 1, 0, 1, 9);
    gridLayout->addWidget(okBtn, 2, 1, 1, 3);
    gridLayout->addWidget(cancelBtn, 2, 5, 1, 3);

    // 构造一些测试数据
#if 0
    QIcon defaultIcon(":/resource/image/avatar.jpg");
    for(int i = 0;i < 10;i++)
    {
        this->addSelectedFriend(QString("1000") + QString::number(i),defaultIcon,"可怡宝宝" + QString::number(i));
    }
#endif

}

void ChooseFriendDialog::addFriendItem(const QString& userId, const QIcon &avatar, const QString &name, bool checked)
{
    ChooseFriendItem* chooseFriendItem = new ChooseFriendItem(this,userId,avatar,name,checked);
    totalContainer->layout()->addWidget(chooseFriendItem);
}

void ChooseFriendDialog::addSelectedFriend(const QString& userId, const QIcon &avatar, const QString &name)
{
    ChooseFriendItem* item = new ChooseFriendItem(this,userId,avatar,name,true);
    selectedContainer->layout()->addWidget(item);
}

void ChooseFriendDialog::deleteSelectedFriend(const QString &userId)
{
    // 遍历 selectedContainer 中的每个Item，如果userId一致，那就删除
    QVBoxLayout* vlayout = dynamic_cast<QVBoxLayout*>(selectedContainer->layout());
    // 遍历删除  所以要从后往前遍历
    for(int i = vlayout->count() - 1;i >= 0;i--)
    {
        auto* item = vlayout->itemAt(i);
        if(item == nullptr || item->widget() == nullptr)
        {
            continue;
        }
        ChooseFriendItem* chooseFriendItem = dynamic_cast<ChooseFriendItem*>(item->widget());
        // 判定当前的元素是否是需要删除的
        if(chooseFriendItem->getUserId() != userId)
        {
            continue;
        }
        vlayout->removeWidget(chooseFriendItem);

        // 此调用不会直接析构对象
        chooseFriendItem->deleteLater();

        // 这里直接delete会导致调用connect的对象直接被析构释放掉，所以程序在运行到connect
        // 该行代码结束后QT的信号槽还会有一系列的操作，但是此时的信号槽调用对象已经被释放掉了
        // 所以会导致程序崩溃。这里的deleteLater就可以很好的保证在信号槽结束之后再释放对象
        // delete chooseFriendItem;
    }
    // 再遍历一遍左侧，将右侧已经删除的元素 对应左侧元素的勾选取消掉
    QVBoxLayout* layoutLeft = dynamic_cast<QVBoxLayout*>(totalContainer->layout());
    for(int i = 0;i < layoutLeft->count();i++)
    {
        auto* item = layoutLeft->itemAt(i);
        if(item == nullptr || item->widget() == nullptr)
        {
            continue;
        }
        ChooseFriendItem* chooseFriendItem = dynamic_cast<ChooseFriendItem*>(item->widget());
        // 判定当前的元素是否是需要取消的
        if(chooseFriendItem->getUserId() != userId)
        {
            continue;
        }
        // 取消选中状态
        chooseFriendItem->getCheckBox()->setChecked(false);
    }
}











