#include "loginwidget.h"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

#include "phoneloginwidget.h"
#include "model/data.h"
#include "model/datacenter.h"
#include "mainwidget.h"

using namespace model;

LoginWidget::LoginWidget(QWidget *parent)
    : QWidget{parent}
{
    // 1.设置窗口基本属性
    this->setFixedSize(400,350);
    this->setWindowTitle("登录窗口");
    this->setWindowIcon(QIcon(":/resource/image/logo.png"));
    this->setStyleSheet("QWidget { background-color: rgb(255,255,255); }");
    this->setAttribute(Qt::WA_DeleteOnClose);

    // 2.创建布局管理器
    QGridLayout* layout = new QGridLayout();
    layout->setSpacing(0);
    layout->setContentsMargins(50,0,50,0);
    this->setLayout(layout);

    // 3.创建标题
    titleLabel = new QLabel();
    titleLabel->setText("登录");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setFixedHeight(50);
    titleLabel->setStyleSheet("QLabel {font-size: 40px; font-weight: 600; }");

    // 4.创建用户名输入框
    QString editStyle = "QLineEdit { border: none; border-radius: 10px; font-size: 20px; background-color: rgb(240, 240, 240); padding-left: 5px;}";
    usernameEdit = new QLineEdit();
    usernameEdit->setFixedHeight(40);
    usernameEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    usernameEdit->setPlaceholderText("输入用户名");
    usernameEdit->setStyleSheet(editStyle);

    // 5. 创建密码输入框
    passwordEdit = new QLineEdit();
    passwordEdit->setFixedHeight(40);
    passwordEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    passwordEdit->setPlaceholderText("输入密码");
    passwordEdit->setStyleSheet(editStyle);
    passwordEdit->setEchoMode(QLineEdit::Password);

    // 6. 创建验证码输入框
    verifyCodeEdit = new QLineEdit();
    verifyCodeEdit->setFixedHeight(40);
    verifyCodeEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    verifyCodeEdit->setPlaceholderText("输入验证码");
    verifyCodeEdit->setStyleSheet(editStyle);

    // // 7. 创建显示验证码图片的控件（此处先用 QPushButton 来表示一下，后续进一步编写这里的逻辑）
    // //    后续会自定义 QWidget，通过画图 api 来实现这里的验证码功能.
    // QPushButton* verifyCodeWidget = new QPushButton();
    // verifyCodeWidget->setText("验证码");
    // verifyCodeWidget->setStyleSheet("QWidget { border: none; }");
    // 7.此时可以直接将已画好的验证码控件放置到这里
    verifyCodeWidget = new VerifyCodeWidget(this);

    // 8. 创建登录按钮
    submitBtn = new QPushButton();
    submitBtn->setText("登录");
    submitBtn->setFixedHeight(40);
    submitBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QString btnGreenStyle = "QPushButton { border: none; border-radius: 10px; background-color: rgb(44, 182, 61); color: rgb(255,255,255); }";
    btnGreenStyle += "QPushButton:pressed { background-color: rgb(240, 240, 240); }";
    submitBtn->setStyleSheet(btnGreenStyle);

    // 9. 创建切换到手机号登录按钮
    phoneModeBtn = new QPushButton();
    phoneModeBtn->setFixedSize(100, 40);
    phoneModeBtn->setText("手机号登录");
    QString btnWhiteStyle = "QPushButton { border: none; border-radius: 10px; background-color: transparent; }";
    btnWhiteStyle += "QPushButton:pressed { background-color: rgb(240, 240, 240); }";
    phoneModeBtn->setStyleSheet(btnWhiteStyle);

    // 10.创建切换模式按钮
    switchModeBtn = new QPushButton();
    switchModeBtn->setFixedSize(100, 40);
    switchModeBtn->setText("注册");
    switchModeBtn->setStyleSheet(btnWhiteStyle);

    // 11. 添加到布局管理器中
    layout->addWidget(titleLabel, 0, 0, 1, 5);
    layout->addWidget(usernameEdit, 1, 0, 1, 5);
    layout->addWidget(passwordEdit, 2, 0, 1, 5);
    layout->addWidget(verifyCodeEdit, 3, 0, 1, 4);
    layout->addWidget(verifyCodeWidget, 3, 4, 1, 1);
    layout->addWidget(submitBtn, 4, 0, 1, 5);
    layout->addWidget(phoneModeBtn, 5, 0, 1, 1);
    layout->addWidget(switchModeBtn, 5, 4, 1, 1);

    // 12. 处理信号槽
    connect(switchModeBtn, &QPushButton::clicked, this, &LoginWidget::switchMode);

    // 切换到电话号码注册/登录界面
    connect(phoneModeBtn,&QPushButton::clicked,this,[=](){
        // 显示新窗口
        PhoneLoginWidget* phoneLoginWidget = new PhoneLoginWidget(nullptr);
        phoneLoginWidget->show();
        // 关闭原窗口
        this->close();
    });
    connect(submitBtn,&QPushButton::clicked,this,&LoginWidget::onClickSubmitBtn);

}

void LoginWidget::switchMode()
{
    if (isLoginMode) {
        // 当前是登录模式，切换到注册模式
        this->setWindowTitle("注册");
        titleLabel->setText("注册");
        submitBtn->setText("注册");
        phoneModeBtn->setText("手机号注册");
        switchModeBtn->setText("登录");
    } else {
        // 当前是注册模式，切换到登录模式
        this->setWindowTitle("登录");
        titleLabel->setText("登录");
        submitBtn->setText("登录");
        phoneModeBtn->setText("手机号登录");
        switchModeBtn->setText("注册");
    }
    isLoginMode = !isLoginMode;
}

void LoginWidget::onClickSubmitBtn()
{
    // 1.取出三个输入框中的内容
    const QString& username = usernameEdit->text();
    const QString& password = passwordEdit->text();
    const QString& verifycode = verifyCodeEdit->text();
    if(username.isEmpty() || password.isEmpty() || verifycode.isEmpty())
    {
        LOG() << "用户名/密码/验证码不能为空！";
        return;
    }

    // 2.判断验证码是否正确
    if(!verifyCodeWidget->cheakVerifyCode(verifycode))
    {
        verifyCodeWidget->refrushVerifyCode();
        LOG() << "验证码不正确，请重新输入！";
        return;
    }

    // 3.发送网络请求
    DataCenter* dataCenter = DataCenter::getInstance();
    if(isLoginMode)
    {
        // 登录逻辑
        connect(dataCenter,&DataCenter::userLoginDone,this,&LoginWidget::userLoginDone);
        dataCenter->userLoginAsync(username,password);
    }else{
        // 注册逻辑
        connect(dataCenter,&DataCenter::userRegisterDone,this,&LoginWidget::userRegisterDone);
        dataCenter->userRegisterAsync(username,password);
    }
}

void LoginWidget::userLoginDone(bool ok,const QString& reason)
{
    // 1.判定是否登录成功
    if(!ok)
    {

        return;
    }

    // 2.登录成功，跳转到主界面
    MainWidget* mainWidget = MainWidget::getInstance();
    // 关闭旧窗口打开窗口
    mainWidget->show();
    this->close();
}

void LoginWidget::userRegisterDone(bool ok, const QString &reason)
{
    if(!ok)
    {

        return;
    }

    // 切换到登录界面
    switchMode();

    // 输入框清空(一般只需要清空验证码框，因为用户注册和登录就是同一个人在操作，没必要都清空)
    verifyCodeEdit->clear();
    verifyCodeWidget->refrushVerifyCode();
}










