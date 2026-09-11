  #include "sessiondetailwidget.h"

#include <QPushButton>
#include <QVBoxLayout>

#include "debug.h"
#include "choosefrienddialog.h"

////////////////////////////////////////////////
/// 表示整个"单聊会话详情"窗口
////////////////////////////////////////////////

SessionDetailWidget::SessionDetailWidget(QWidget* parent)
    :QDialog(parent)
{
    // 1.设置一些基本属性
    this->setWindowTitle("会话详情");
    this->setFixedSize(300,200);
    this->setWindowIcon(QIcon(":/resource/image/logo.png"));
    this->setAttribute(Qt::WA_DeleteOnClose);

    // 2.创建布局管理器
    QGridLayout* layout = new QGridLayout();
    layout->setContentsMargins(50,0,50,0);
    layout->setSpacing(10);
    layout->setAlignment(Qt::AlignHCenter);
    this->setLayout(layout);

    // 3.创建"添加群聊"按钮
    AvatarItem* createGroupBtn = new AvatarItem(QIcon(":/resource/image/cross.png"),"添加");
    layout->addWidget(createGroupBtn,0,0);

    // 4.添加用户信息，创建一些假数据
#if TEST_UI

    AvatarItem* currentUser = new AvatarItem(QIcon(":/resource/image/avatar.jpg"),"张三");
    layout->addWidget(currentUser,0,1);

#endif

    // 5.添加"删除好友"按钮
    deleteFriendBtn = new QPushButton();
    deleteFriendBtn->setFixedHeight(50);
    deleteFriendBtn->setText("删除好友");
    QString style = "QPushButton { border: 1px solid rgb(90,90,90); border-radius: 5px; }";
    style += "QPushButton:pressed { background-color: rgb(235,235,235); }";
    deleteFriendBtn->setStyleSheet(style);
    layout->addWidget(deleteFriendBtn,1,0,1,3);

    // 6.连接信号槽处理添加按钮信号
    connect(createGroupBtn->getAvatarBtn(),&QPushButton::clicked,this,[=](){
        ChooseFriendDialog* chooseFriendDialog = new ChooseFriendDialog(this);
        chooseFriendDialog->exec();
    });
}

////////////////////////////////////////////////
/// 表示一个头像 + 一个名字组合控件
////////////////////////////////////////////////

AvatarItem::AvatarItem(const QIcon &avatar, const QString &name)
{
    // 1.设置自身基本属性
    this->setFixedSize(120,80);

    // 2.创建垂直布局管理器
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    layout->setAlignment(Qt::AlignHCenter);
    this->setLayout(layout);

    // 3.创建头像按钮
    avatarBtn = new QPushButton();
    avatarBtn->setFixedSize(45,45);
    avatarBtn->setIconSize(QSize(45,45));
    avatarBtn->setIcon(avatar);
    avatarBtn->setStyleSheet("QPushButton { border: none; }");

    // 4. 创建名字
    nameLabel = new QLabel();
    nameLabel->setText(name);
    QFont font("微软雅黑", 12);
    nameLabel->setFont(font);
    nameLabel->setAlignment(Qt::AlignCenter);

    // 5. 对名字做 "截断操作"
    const int MAX_WIDTH = 65;
    QFontMetrics metrics(font);
    int totalWidth = metrics.horizontalAdvance(name);
    if (totalWidth >= MAX_WIDTH) {
        // 需要截断
        QString tail = "...";
        int tailWidth = metrics.horizontalAdvance(tail);
        int availableWidth = MAX_WIDTH - tailWidth;
        int availableSize = name.size() * ((double)availableWidth / totalWidth);
        QString newName = name.left(availableSize);
        nameLabel->setText(newName + tail);
    }

    // 6.将控件添加到布局管理器中
    layout->addWidget(avatarBtn);
    layout->addWidget(nameLabel);

}














