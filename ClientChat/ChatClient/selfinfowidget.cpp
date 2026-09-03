#include "selfinfowidget.h"

#include<QGridLayout>

#include "debug.h"

SelfInfoWidget::SelfInfoWidget(QWidget* parent) : QDialog(parent)
{
    // 1.设置整个窗口的属性
    this->setFixedSize(500,300);
    this->setWindowTitle("个人信息");

    // 把对话框移动到鼠标点击的位置
    this->move(QCursor::pos());

    // 2.创建网格布局管理器
    QGridLayout* layout = new QGridLayout();
    //layout->setSpacing(0); // 默认设置布局管理器内所有控件的水平、垂直间距;也可以分开设置
    layout->setHorizontalSpacing(10);
    layout->setVerticalSpacing(5);
    layout->setContentsMargins(20,20,20,0);
    layout->setAlignment(Qt::AlignTop); // 靠上对齐
    this->setLayout(layout);

    QString labelStyle = "QLabel { font-size: 14px; font-weight: 800; }";
    QString btnStyle = "QPushButton { border: none; background-color: transparent; }";
    btnStyle += "QPushButton:pressed { background-color: rgb(210,210,210); }";
    QString editStyle = "QLineEdit { border: none; border-radius: 5px; padding-left: 3px; }";

    int height = 30;

    // 3.创建头像
    avatarBtn = new QPushButton();
    avatarBtn->setFixedSize(75,75);
    avatarBtn->setIconSize(QSize(75,75));
    avatarBtn->setIcon(QIcon(":/resource/image/avatar.jpg"));
    avatarBtn->setStyleSheet("QPusuButton { border: none; background-color: transparent; }");
    layout->addWidget(avatarBtn,0,0,3,1);

    // 4.添加用户id显示
    idTag = new QLabel();
    idTag->setFixedSize(50,height);
    idTag->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    idTag->setText("序号");
    idTag->setStyleSheet(labelStyle);

    idLabel = new QLabel();
    idLabel->setFixedHeight(height);
    idLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    // 5.添加用户的名字显示
    nameTag = new QLabel();
    nameTag->setFixedSize(50,height);
    nameTag->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    nameTag->setText("昵称");
    nameTag->setStyleSheet(labelStyle);

    nameLabel = new QLabel();
    nameLabel->setFixedHeight(height);
    nameLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    nameModifyBtn = new QPushButton();
    nameModifyBtn->setFixedSize(25, 25);
    nameModifyBtn->setIconSize(QSize(20, 20));
    nameModifyBtn->setIcon(QIcon(":/resource/image/modify.png"));
    nameModifyBtn->setStyleSheet(btnStyle);

    nameEdit = new QLineEdit();
    nameEdit->setFixedHeight(height);
    nameEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    nameEdit->setStyleSheet(editStyle);
    nameEdit->hide();

    nameSubmitBtn = new QPushButton();
    nameSubmitBtn->setFixedSize(25, 25);
    nameSubmitBtn->setIconSize(QSize(20, 20));
    nameSubmitBtn->setIcon(QIcon(":/resource/image/submit.png"));
    nameSubmitBtn->setStyleSheet(btnStyle);
    nameSubmitBtn->hide();

    // 6.添加个性签名显示
    descTag = new QLabel();
    descTag->setFixedSize(50,height);
    descTag->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    descTag->setText("签名");
    descTag->setStyleSheet(labelStyle);

    descLabel = new QLabel();
    descLabel->setFixedHeight(height);
    descLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    descModifyBtn = new QPushButton();
    descModifyBtn->setFixedSize(25,25);
    descModifyBtn->setIconSize(QSize(20,20));
    descModifyBtn->setIcon(QIcon(":/resource/image/modify.png"));
    descModifyBtn->setStyleSheet(btnStyle);

    descEdit = new QLineEdit();
    descEdit->setFixedHeight(height);
    descEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    descEdit->setStyleSheet(editStyle);
    descEdit->hide();

    descSubmitBtn = new QPushButton();
    descSubmitBtn->setFixedSize(25, 25);
    descSubmitBtn->setIconSize(QSize(20, 20));
    descSubmitBtn->setIcon(QIcon(":/resource/image/submit.png"));
    descSubmitBtn->setStyleSheet(btnStyle);
    descSubmitBtn->hide();

    // 7. 添加电话
    phoneTag = new QLabel();
    phoneTag->setFixedSize(50,height);
    phoneTag->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    phoneTag->setText("电话");
    phoneTag->setStyleSheet(labelStyle);

    phoneLabel = new QLabel();
    phoneLabel->setFixedHeight(height);
    phoneLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    phoneModifyBtn = new QPushButton();
    phoneModifyBtn->setFixedSize(25, 25);
    phoneModifyBtn->setIconSize(QSize(20, 20));
    phoneModifyBtn->setIcon(QIcon(":/resource/image/modify.png"));
    phoneModifyBtn->setStyleSheet(btnStyle);

    phoneEdit = new QLineEdit();
    phoneEdit->setFixedHeight(height);
    phoneEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    phoneEdit->setStyleSheet(editStyle);
    phoneEdit->hide();

    phoneSubmitBtn = new QPushButton();
    phoneSubmitBtn->setFixedSize(25, 25);
    phoneSubmitBtn->setIconSize(QSize(20, 20));
    phoneSubmitBtn->setIcon(QIcon(":/resource/image/submit.png"));
    phoneSubmitBtn->setStyleSheet(btnStyle);
    phoneSubmitBtn->hide();

    // 8. 添加验证码
    verifyCodeTag = new QLabel();
    verifyCodeTag->setFixedSize(50,height);
    verifyCodeTag->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    verifyCodeTag->setText("验证码");
    verifyCodeTag->setStyleSheet(labelStyle);
    verifyCodeTag->hide();

    verifyCodeEdit = new QLineEdit();
    verifyCodeEdit->setFixedHeight(height);
    verifyCodeEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    verifyCodeEdit->setStyleSheet(editStyle);
    verifyCodeEdit->setPlaceholderText("请输入验证码");
    verifyCodeEdit->hide();

    getVerifyCodeBtn = new QPushButton();
    getVerifyCodeBtn->setText("获取验证码");
    getVerifyCodeBtn->setStyleSheet("QPushButton { border: none; border-radius: 5px; background-color: transparent; } QPushButton:pressed { background-color: rgb(231,231,231);}");
    getVerifyCodeBtn->setFixedSize(70,height);
    getVerifyCodeBtn->hide();


    // 9.将控件都放入布局管理器  第 0 列被头像占用了，下列内容都是从第一列开始
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

    // 10.连接信号槽
    connect(nameModifyBtn,&QPushButton::clicked,this,[=](){
        // 把当前的nameLabel和nameModifyBtn隐藏起来，并将它们移除出布局管理器
        nameLabel->hide();
        nameModifyBtn->hide();
        layout->removeWidget(nameLabel);
        layout->removeWidget(nameModifyBtn);
        // 将nameEdit和nameSubmit显示出来
        nameEdit->show();
        nameSubmitBtn->show();
        layout->addWidget(nameEdit,1,2);
        layout->addWidget(nameSubmitBtn,1,3);
        // 把输入框的内容进行设置
        nameEdit->setText(nameLabel->text());

    });

    connect(descModifyBtn, &QPushButton::clicked, this, [=]() {
        descLabel->hide();
        descModifyBtn->hide();
        layout->removeWidget(descLabel);
        layout->removeWidget(descModifyBtn);

        descEdit->show();
        descSubmitBtn->show();
        layout->addWidget(descEdit, 2, 2);
        layout->addWidget(descSubmitBtn, 2, 3);

        descEdit->setText(descLabel->text());
    });

    connect(phoneModifyBtn, &QPushButton::clicked, this, [=]() {
        phoneLabel->hide();
        phoneModifyBtn->hide();
        layout->removeWidget(phoneLabel);
        layout->removeWidget(phoneModifyBtn);

        phoneEdit->show();
        phoneSubmitBtn->show();
        layout->addWidget(phoneEdit, 3, 2);
        layout->addWidget(phoneSubmitBtn, 3, 3);

        phoneEdit->setText(phoneLabel->text());

        // 验证码相关控件默认不显示，只有当点击"编辑电话号码"按钮时才显示出来
        verifyCodeTag->show();
        verifyCodeEdit->show();
        getVerifyCodeBtn->show();
        layout->addWidget(verifyCodeTag,4,1);
        layout->addWidget(verifyCodeEdit,4,2);
        layout->addWidget(getVerifyCodeBtn,4,3);
    });

    // 点击关闭对话框后要及时释放掉对象内存以免内存泄露
    this->setAttribute(Qt::WA_DeleteOnClose);
}








