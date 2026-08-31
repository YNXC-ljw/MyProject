#include "selfinfowidget.h"

#include<QGridLayout>

#include "debug.h"

SelfInfoWidget::SelfInfoWidget(QWidget* parent) : QDialog(parent)
{
    // 1.设置整个窗口的属性
    this->setFixedSize(450,300);
    this->setWindowTitle("个人信息");

    // 把对话框移动到鼠标点击的位置
    this->move(QCursor::pos());

    // 2.创建网格布局管理器
    QGridLayout* layout = new QGridLayout();
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    this->setLayout(layout);

    QString labelStyle = "QLabel { font-size: 14px; font-weight: 800; }";
    QString btnStyle = "QPushButton { border: none; background-color: transparent; }";
    btnStyle += "QPushButton:pressed { background-color: rgb(210,210,210); }";

    // 3.创建头像
    avatarBtn = new QPushButton();
    avatarBtn->setFixedSize(75,75);
    avatarBtn->setIconSize(QSize(75,75));
    avatarBtn->setIcon(QIcon(":/resource/image/avatar.jpg"));
    avatarBtn->setStyleSheet("QPusuButton { border: none; background-color: transparent; }");
    layout->addWidget(avatarBtn,0,0,3,1);

    // 4.添加用户id显示
    idTag = new QLabel();
    idTag->setFixedWidth(50);
    idTag->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
    idTag->setText("序号");
    idTag->setStyleSheet(labelStyle);

    idLabel = new QLabel();
    idLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    // 5.添加用户的名字显示
    nameTag = new QLabel();
    nameTag->setFixedWidth(50);
    nameTag->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    nameTag->setText("昵称");
    nameTag->setStyleSheet(labelStyle);

    nameLabel = new QLabel();
    nameLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    nameModifyBtn = new QPushButton();
    nameModifyBtn->setFixedSize(25, 25);
    nameModifyBtn->setIconSize(QSize(20, 20));
    nameModifyBtn->setIcon(QIcon(":/resource/image/modify.png"));
    nameModifyBtn->setStyleSheet(btnStyle);

    nameEdit = new QLineEdit();
    nameEdit->hide();

    nameSubmitBtn = new QPushButton();
    nameSubmitBtn->setFixedSize(25, 25);
    nameSubmitBtn->setIconSize(QSize(20, 20));
    nameSubmitBtn->setIcon(QIcon(":/resource/image/submit.png"));
    nameSubmitBtn->setStyleSheet(btnStyle);
    nameSubmitBtn->hide();

    // 6.添加个性签名显示
    descTag = new QLabel();
    descTag->setFixedWidth(50);
    descTag->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
    descTag->setText("签名");
    descTag->setStyleSheet(labelStyle);

    descLabel = new QLabel();
    descLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    descModifyBtn = new QPushButton();
    descModifyBtn->setFixedSize(25,25);
    descModifyBtn->setIconSize(QSize(20,20));
    descModifyBtn->setIcon(QIcon(":/resource/image/modify.png"));
    descModifyBtn->setStyleSheet(btnStyle);

    descEdit = new QLineEdit();
    descEdit->hide();

    descSubmitBtn = new QPushButton();
    descSubmitBtn->setFixedSize(25, 25);
    descSubmitBtn->setIconSize(QSize(20, 20));
    descSubmitBtn->setIcon(QIcon(":/resource/image/submit.png"));
    descSubmitBtn->setStyleSheet(btnStyle);
    descSubmitBtn->hide();

    // 7. 添加电话
    phoneTag = new QLabel();
    phoneTag->setFixedWidth(50);
    phoneTag->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    phoneTag->setText("电话");
    phoneTag->setStyleSheet(labelStyle);

    phoneLabel = new QLabel();
    phoneLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    phoneModifyBtn = new QPushButton();
    phoneModifyBtn->setFixedSize(25, 25);
    phoneModifyBtn->setIconSize(QSize(20, 20));
    phoneModifyBtn->setIcon(QIcon(":/resource/image/modify.png"));
    phoneModifyBtn->setStyleSheet(btnStyle);

    phoneEdit = new QLineEdit();
    phoneEdit->hide();

    phoneSubmitBtn = new QPushButton();
    phoneSubmitBtn->setFixedSize(25, 25);
    phoneSubmitBtn->setIconSize(QSize(20, 20));
    phoneSubmitBtn->setIcon(QIcon(":/resource/image/submit.png"));
    phoneSubmitBtn->setStyleSheet(btnStyle);
    phoneSubmitBtn->hide();


    // 第 0 列被头像占用了，下列内容都是从第一列开始
    layout->addWidget(idTag,0,1);
    layout->addWidget(idLabel,0,2);

    layout->addWidget(nameTag,1,1);
    layout->addWidget(nameLabel,1,2);
    layout->addWidget(nameModifyBtn,1,3);

    layout->addWidget(descTag,2,1);
    layout->addWidget(descLabel,2,2);
    layout->addWidget(descModifyBtn,2,3);

    layout->addWidget(phoneTag,3,1);
    layout->addWidget(phoneLabel,3,2);
    layout->addWidget(phoneModifyBtn,3,3);




#if TEST_UI
    idLabel->setText("1234");
    nameLabel->setText("可怡宝宝");
    descLabel->setText("可怡宝宝最可爱了");
    phoneLabel->setText("18212345678");


#endif

    // 点击关闭对话框后要及时释放掉对象内存以免内存泄露
    this->setAttribute(Qt::WA_DeleteOnClose);
}








