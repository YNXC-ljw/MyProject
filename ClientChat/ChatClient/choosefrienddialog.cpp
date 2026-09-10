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

ChooseFriendItem::ChooseFriendItem(const QIcon &avatar, const QString &name, bool checked)
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
    layout->addWidget(scrollArea);

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
        this->addFriendItem(defaultAvatar, "张三" + QString::number(i), false);
    }
#endif

}

void ChooseFriendDialog::initRight(QHBoxLayout *layout)
{
    // 1. 创建右侧的布局管理器
    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setSpacing(0);
    layout->addLayout(gridLayout);

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

}

void ChooseFriendDialog::addFriendItem(const QIcon &avatar, const QString &name, bool checked)
{
    ChooseFriendItem* chooseFriendItem = new ChooseFriendItem(avatar,name,checked);
    totalContainer->layout()->addWidget(chooseFriendItem);
}











