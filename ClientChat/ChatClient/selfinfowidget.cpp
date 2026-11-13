#include "selfinfowidget.h"
#include "model/datacenter.h"

#include<QGridLayout>
#include <QFileDialog>
#include <QTimer>

#include "debug.h"

using namespace model;

SelfInfoWidget::SelfInfoWidget(QWidget* parent) : QDialog(parent)
{
    // 1.设置整个窗口的属性
    this->setFixedSize(500,300);
    this->setWindowTitle("个人信息");

    // 把对话框移动到鼠标点击的位置
    this->move(QCursor::pos());

    // 2.创建网格布局管理器
    layout = new QGridLayout();
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
    avatarBtn->setIcon(QIcon(":/resource/image/avatar.jpg"));
#endif

    // 10.连接信号槽
    initSignalSlots();

    // 点击关闭对话框后要及时释放掉对象内存以免内存泄露
    this->setAttribute(Qt::WA_DeleteOnClose);

    // 11.将本地数据加载到界面上
    DataCenter* dataCenter = DataCenter::getInstance();
    UserInfo* myself = dataCenter->getMyself();
    if(myself != nullptr)
    {
        // 把本地数据加载到个人信息上
        avatarBtn->setIcon(myself->avatar);
        idLabel->setText(myself->userId);
        nameLabel->setText(myself->nickName);
        descLabel->setText(myself->description);
        phoneLabel->setText(myself->phone);
    }
}

void SelfInfoWidget::initSignalSlots()
{
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

    // 修改昵称
    connect(nameSubmitBtn,&QPushButton::clicked,this,&SelfInfoWidget::clickNameSubmitBtn);
    // 修改签名
    connect(descSubmitBtn,&QPushButton::clicked,this,&SelfInfoWidget::clickDescSubmitBtn);
    // 修改手机号
    connect(phoneSubmitBtn,&QPushButton::clicked,this,&SelfInfoWidget::clickPhoneSubmitBtn);
    // 获取验证码
    connect(getVerifyCodeBtn,&QPushButton::clicked,this,&SelfInfoWidget::clickVerifyCodeBtn);
    // 修改头像
    connect(avatarBtn,&QPushButton::clicked,this,&SelfInfoWidget::clickAvatarBtn);
}

void SelfInfoWidget::clickNameSubmitBtn()
{
    // 1.从输入框中拿到昵称内容
    const QString& nickname = nameEdit->text();
    if(nickname.isEmpty())
    {
        return;
    }
    // 2.发送网络请求
    DataCenter *dataCenter = DataCenter::getInstance();
    connect(dataCenter,&DataCenter::changeNickNameDone,this,&SelfInfoWidget::onClickNameSubmitBtn);
    dataCenter->changeNickNameAsync(nickname);
}

void SelfInfoWidget::onClickNameSubmitBtn()
{
    // 对界面控件进行切换．把刚才输入框切换回 label，把提交按钮切换回编辑按钮．
    // 同时还需要把输入框中的本文设置为 label 中的文本．
    layout->removeWidget(nameEdit);
    nameEdit->hide();
    layout->addWidget(nameLabel, 1, 2);
    nameLabel->show();
    nameLabel->setText(nameEdit->text());

    layout->removeWidget(nameSubmitBtn);
    nameSubmitBtn->hide();
    layout->addWidget(nameModifyBtn, 1, 3);
    nameModifyBtn->show();

}

void SelfInfoWidget::clickDescSubmitBtn()
{
    // 1.从输入框中拿到昵称内容
    const QString& description = descEdit->text();
    if(description.isEmpty())
    {
        return;
    }
    // 2.发送网络请求
    DataCenter *dataCenter = DataCenter::getInstance();
    connect(dataCenter,&DataCenter::changeDescptionDone,this,&SelfInfoWidget::onClickDescSubmitBtn);
    dataCenter->changeDesciptionAsync(description);
}

void SelfInfoWidget::onClickDescSubmitBtn()
{
    // 切换界面.
    // 把 label 替换回输入框，把编辑按钮替换回修改按钮
    layout->removeWidget(descEdit);
    descEdit->hide();
    layout->addWidget(descLabel, 2, 2);
    descLabel->show();
    descLabel->setText(descEdit->text());

    layout->removeWidget(descSubmitBtn);
    descSubmitBtn->hide();
    layout->addWidget(descModifyBtn, 2, 3);
    descModifyBtn->show();

}

void SelfInfoWidget::clickPhoneSubmitBtn()
{
    // 1.先确认验证码是否已经收到
    DataCenter* dataCenter = DataCenter::getInstance();
    QString verifyCodeId = dataCenter->getVerifyCodeId();
    if(verifyCodeId.isEmpty())
    {
        LOG() << "服务器尚未返回验证码，请稍候重试！";
        return;
    }
    // 如果当前已经拿到 verifyCodeId，就可以清空 DataCenter 中存储的值，确保下次点击提交按钮的时候，上述逻辑仍然有效
    dataCenter->resetVerifyCodeId("");

    // 2. 获取到用户输入的验证码
    const QString& verifyCode = verifyCodeEdit->text();
    if (verifyCode.isEmpty()) {
        // Toast::showMessage("验证码不能为空！");
        return;
    }
    verifyCodeEdit->setText("");

    // 3.发送请求
    connect(dataCenter,&DataCenter::changePhoneNumberDone,this,&SelfInfoWidget::onClickPhoneSubmitBtn);
    dataCenter->changePhoneAsync(newPhone,verifyCodeId,verifyCode);

    //4. 让验证码按钮的倒计时停止，把 leftTime 设为 1，就可以停止了
    time = 1;
}

void SelfInfoWidget::onClickPhoneSubmitBtn()
{
    // 清除验证码相关控件并还原界面
    layout->removeWidget(verifyCodeEdit);
    layout->removeWidget(verifyCodeTag);
    layout->removeWidget(getVerifyCodeBtn);
    layout->removeWidget(phoneEdit);
    layout->removeWidget(phoneSubmitBtn);
    verifyCodeTag->hide();
    verifyCodeEdit->hide();
    getVerifyCodeBtn->hide();
    phoneEdit->hide();
    phoneSubmitBtn->hide();

    layout->addWidget(phoneLabel, 3, 2);
    phoneLabel->show();
    layout->addWidget(phoneModifyBtn, 3, 3);
    phoneModifyBtn->show();
    phoneLabel->setText(newPhone);
}

void SelfInfoWidget::clickVerifyCodeBtn()
{
    // 1.获取到输入框中的手机号
    const QString& phone = phoneEdit->text();
    if(phone.isEmpty())
    {
        return;
    }
    // 2.通过网络给服务器发送请求
    DataCenter* dataCenter = DataCenter::getInstance();
    connect(dataCenter,&DataCenter::getVerifyCodeDone,this,[=](){
       // 不需要做任何处理，只需要提示一下验证码已经发送

    });
    dataCenter->getVerifyCodeAsync(phone);

    // 3.先将需要修改的目标手机号存起来
    // 后续修改的手机号就不从输入框中读取(以免获取验证码期间又修改),直接读取该变量
    this->newPhone = phone;

    // 4.禁用发送验证码按钮，设置倒计时
    getVerifyCodeBtn->setEnabled(false);

    time = 60;
    QTimer* timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,[=](){
        if(time <= 1)
        {
            // 倒计时结束
            getVerifyCodeBtn->setEnabled(true);
            getVerifyCodeBtn->setText("获取验证码");
            timer->stop();
            timer->deleteLater();
            return;
        }
        getVerifyCodeBtn->setText(QString::number(--time) + "s");
    });
    timer->start(1000);
}

void SelfInfoWidget::clickAvatarBtn()
{
    // 1.选择弹出的对话框文件
    QString filter = "Image Files (*.jpg *.png *.jpeg)";
    QString imagePath = QFileDialog::getOpenFileName(this,"选择头像",QDir::homePath(),filter);
    if(imagePath.isEmpty())
    {
        LOG() << "用户未选择头像!";
        return;
    }

    QByteArray imageBytes = readFiletoQByteArray(imagePath);
    // 2.发送请求
    DataCenter* dataCenter = DataCenter::getInstance();
    dataCenter->changeAvatarAsync(imageBytes);
}








