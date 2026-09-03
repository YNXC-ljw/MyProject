#include "userinfowidget.h"

#include <QGridLayout>


UserInfoWidget::UserInfoWidget(const UserInfo& userInfo,QWidget* parent)
    :QDialog(parent)
    ,userInfo(userInfo)
{
    // 1.设置基本属性
    this->setFixedSize(340,200);
    this->setWindowIcon(QIcon(":/resource/image/logo.png"));
    this->setWindowTitle("用户详细信息");
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->move(QCursor::pos());

    // 2.创建布局管理器
    QGridLayout* layout = new QGridLayout();
    layout->setHorizontalSpacing(20);
    layout->setVerticalSpacing(10);
    layout->setContentsMargins(30,0,0,0);
    layout->setAlignment(Qt::AlignTop);
    this->setLayout(layout);


    QString labelStyle = "QLabel { font-weight: 800; padding-left: 20px; }";
    QString btnStyle = "QPushButton { border: 1px solid rdb(100,100,100); border-radius: 5px; background-color: rgb(240,240,240); }";
    btnStyle += "QPushButton:pressed { background-color: rgb(205,205,205); }";

    int height = 30;
    int width = 80;

    // 3.添加头像
    avatarBtn = new QPushButton();
    avatarBtn->setFixedSize(75,75);
    avatarBtn->setIconSize(QSize(75,75));
    avatarBtn->setIcon(userInfo.avatar);
    avatarBtn->setStyleSheet("QPushButton { border: none; background-color: transparent; }");

    // 4.添加用户序号
    idTag = new QLabel();
    idTag->setText("序号");
    idTag->setStyleSheet(labelStyle);
    idTag->setFixedSize(width,height);
    idTag->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    idLabel = new QLabel();
    idLabel->setText(userInfo.userId);
    idLabel->setFixedSize(width,height);
    idLabel->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    // 5.添加用户昵称
    nameTag = new QLabel();
    nameTag->setText("昵称");
    nameTag->setStyleSheet(labelStyle);
    nameTag->setFixedSize(width,height);
    nameTag->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    nameLabel = new QLabel();
    nameLabel->setText(userInfo.nickName);
    nameLabel->setFixedSize(width,height);
    nameLabel->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    // 6. 设置电话
    phoneTag = new QLabel();
    phoneTag->setText("电话");
    phoneTag->setStyleSheet(labelStyle);
    phoneTag->setFixedSize(width,height);
    phoneTag->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    phoneLabel = new QLabel();
    phoneLabel->setText(userInfo.phone);
    phoneLabel->setFixedSize(width,height);
    phoneLabel->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    // 7. 添加功能按钮
    applyBtn = new QPushButton();
    applyBtn->setText("申请好友");
    applyBtn->setFixedSize(80, 30);
    applyBtn->setStyleSheet(btnStyle);

    sendMessageBtn = new QPushButton();
    sendMessageBtn->setText("发送消息");
    sendMessageBtn->setFixedSize(80, 30);
    sendMessageBtn->setStyleSheet(btnStyle);

    deleteFriendBtn = new QPushButton();
    deleteFriendBtn->setText("删除好友");
    deleteFriendBtn->setFixedSize(80, 30);
    deleteFriendBtn->setStyleSheet(btnStyle);

    // 8.将上述控件添加到布局管理器中
    layout->addWidget(avatarBtn, 0, 0, 3, 1);

    layout->addWidget(idTag, 0, 1);
    layout->addWidget(idLabel, 0, 2);

    layout->addWidget(nameTag, 1, 1);
    layout->addWidget(nameLabel, 1, 2);

    layout->addWidget(phoneTag, 2, 1);
    layout->addWidget(phoneLabel, 2, 2);

    layout->addWidget(applyBtn, 3, 0);
    layout->addWidget(sendMessageBtn, 3, 1);
    layout->addWidget(deleteFriendBtn, 3, 2);

}









