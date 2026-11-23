#include "messageeditarea.h"
#include "historymessagewidget.h"
#include "model/datacenter.h"
#include "sessionshowarea.h"
#include "mainwidget.h"
#include "soundrecorder.h"
#include "toast.h"

#include <QVBoxLayout>
#include <QScrollBar>
#include <QHBoxLayout>
#include <QFileDialog>

using namespace model;

MessageEditArea::MessageEditArea(QWidget *parent)
    : QWidget{parent}
{
    this->setAttribute(Qt::WA_StyledBackground, true);
    // 1.设置必要属性
    this->setFixedHeight(200);
    this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    this->setStyleSheet("MessageEditArea {border-radius: 10px; border: 1px solid rgb(227, 227, 227);}");

    // 2.创建垂直方向布局管理器
    QVBoxLayout* vlayout = new QVBoxLayout();
    vlayout->setSpacing(0);
    vlayout->setContentsMargins(10,0,10,0);
    this->setLayout(vlayout);

    // 3.创建水平方向布局管理器
    QHBoxLayout* hlayout = new QHBoxLayout();
    hlayout->setSpacing(0);
    hlayout->setContentsMargins(10,0,0,8);
    hlayout->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
    vlayout->addLayout(hlayout);

    // // 4.创建按钮，将其放入水平布局管理器中
    // QString btnStyle = "QPushButton {background-color: rgb(245,245,245);} QPushButton:pressed {background-color: rgb(255,255,255);}";
    // sendImageBtn = new QPushButton();
    // sendImageBtn->setFixedSize(35,35);
    // sendImageBtn->setIconSize(QSize(25,25));
    // sendImageBtn->setIcon(QIcon(":/resource/image/image.png"));
    // sendImageBtn->setStyleSheet(btnStyle);
    // hlayout->addWidget(sendImageBtn);

    // sendFileBtn = new QPushButton();
    // sendFileBtn->setFixedSize(35,35);
    // sendFileBtn->setIconSize(QSize(25,25));
    // sendFileBtn->setIcon(QIcon(":/resource/image/file.png"));
    // sendFileBtn->setStyleSheet(btnStyle);
    // hlayout->addWidget(sendFileBtn);

    // sendSpeechBtn = new QPushButton();
    // sendSpeechBtn->setFixedSize(35,35);
    // sendSpeechBtn->setIconSize(QSize(25,25));
    // sendSpeechBtn->setIcon(QIcon(":/resource/image/speech.png"));
    // sendSpeechBtn->setStyleSheet(btnStyle);
    // hlayout->addWidget(sendSpeechBtn);

    // showHistoryBtn = new QPushButton();
    // showHistoryBtn->setFixedSize(35,35);
    // showHistoryBtn->setIconSize(QSize(25,25));
    // showHistoryBtn->setIcon(QIcon(":/resource/image/history.png"));
    // showHistoryBtn->setStyleSheet(btnStyle);
    // hlayout->addWidget(showHistoryBtn);

    // hlayout->addStretch();

    // // 5. 添加多行编辑框
    // textEdit = new QPlainTextEdit();
    // textEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    // textEdit->setStyleSheet("QPlainTextEdit { border: none; background-color: transparent; font-size: 14px; padding: 10px;}");
    // textEdit->verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width: 2px; background-color: rgb(45, 45, 45);}");
    // vlayout->addWidget(textEdit);

    // // 6.添加"正在录制中"提示
    // tipLabel = new QLabel();
    // tipLabel->setText("正在录制中...");
    // tipLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    // tipLabel->setAlignment(Qt::AlignCenter);
    // tipLabel->setFont(QFont("微软雅黑",24,600));
    // hlayout->addWidget(tipLabel);
    // tipLabel->hide();

    // // 7. 添加发送文本消息的按钮
    // sendTextBtn = new QPushButton();
    // sendTextBtn->setText("发送");
    // sendTextBtn->setFixedSize(120, 40);
    // QString style = "QPushButton { font-size: 16px; color: rgb(7, 193, 96); border: none; background-color: rgb(233, 233, 233);border-radius: 10px;}";
    // style += "QPushButton:hover { background-color: rgb(210, 210, 210); }";
    // style += "QPushButton:pressed { background-color: rgb(200, 200, 200); }";
    // sendTextBtn->setStyleSheet(style);
    // hlayout->addWidget(sendTextBtn,0,Qt::AlignRight | Qt::AlignVCenter);

    // 4. 把上方的四个按钮, 创建好并添加到水平布局中
    QString btnStyle = "QPushButton { background-color: rgb(245, 245, 245); border: none; } QPushButton:pressed { background-color: rgb(255, 255, 255); }";
    QSize btnSize(35, 35);
    QSize iconSize(25, 25);

    sendImageBtn = new QPushButton();
    sendImageBtn->setFixedSize(btnSize);
    sendImageBtn->setIconSize(iconSize);
    sendImageBtn->setIcon(QIcon(":/resource/image/image.png"));
    sendImageBtn->setStyleSheet(btnStyle);
    hlayout->addWidget(sendImageBtn);

    sendFileBtn = new QPushButton();
    sendFileBtn->setFixedSize(btnSize);
    sendFileBtn->setIconSize(iconSize);
    sendFileBtn->setIcon(QIcon(":/resource/image/file.png"));
    sendFileBtn->setStyleSheet(btnStyle);
    hlayout->addWidget(sendFileBtn);

    sendSpeechBtn = new QPushButton();
    sendSpeechBtn->setFixedSize(btnSize);
    sendSpeechBtn->setIconSize(iconSize);
    sendSpeechBtn->setIcon(QIcon(":/resource/image/speech.png"));
    sendSpeechBtn->setStyleSheet(btnStyle);
    hlayout->addWidget(sendSpeechBtn);

    showHistoryBtn = new QPushButton();
    showHistoryBtn->setFixedSize(btnSize);
    showHistoryBtn->setIconSize(iconSize);
    showHistoryBtn->setIcon(QIcon(":/resource/image/history.png"));
    showHistoryBtn->setStyleSheet(btnStyle);
    hlayout->addWidget(showHistoryBtn);

    // 5. 添加多行编辑框
    textEdit = new QPlainTextEdit();
    textEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    textEdit->setStyleSheet("QPlainTextEdit { border: none; background-color: transparent; font-size: 14px; padding: 10px; }");
    textEdit->verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width: 2px; background-color: rgb(45, 45, 45); }");
    vlayout->addWidget(textEdit);

    // 6. 添加提示 "录制中" 这样的 QLabel
    tipLabel = new QLabel();
    tipLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tipLabel->setText("录音中...");
    tipLabel->setAlignment(Qt::AlignCenter);
    tipLabel->setFont(QFont("微软雅黑", 24, 600));
    vlayout->addWidget(tipLabel);
    tipLabel->hide();

    // 7. 添加发送文本消息的按钮
    sendTextBtn = new QPushButton();
    sendTextBtn->setText("发送");
    sendTextBtn->setFixedSize(120, 40);
    QString style = "QPushButton { font-size: 16px; color: rgb(7, 193, 96); border: none; background-color: rgb(233, 233, 233); border-radius: 10px; } ";
    style += "QPushButton:hover { background-color: rgb(210, 210, 210); }";
    style += "QPushButton:pressed { background-color: rgb(190, 190, 190); }";
    sendTextBtn->setStyleSheet(style);
    vlayout->addWidget(sendTextBtn, 0, Qt::AlignRight | Qt::AlignVCenter);

    // 8.统一处理信号槽
    initSignalSlot();

}

void MessageEditArea::initSignalSlot()
{
    // 1.关联"获取历史消息"按钮信号槽
    DataCenter *dataCenter = DataCenter::getInstance();
    // 显示历史消息窗口
    connect(showHistoryBtn,&QPushButton::clicked,this,[=](){
        if(dataCenter->getCurrentChatSessionId().isEmpty())
        {
            return;
        }
        HistoryMessageWidget* historyMessageWidget = new HistoryMessageWidget(this);
        historyMessageWidget->exec();
    });

    // 2.关联"发送文本消息"信号槽
    connect(sendTextBtn,&QPushButton::clicked,this,&MessageEditArea::sendTextMessage);
    connect(dataCenter,&DataCenter::sendMessageDone,this,&MessageEditArea::addSelfMessage);

    // 3.关联收到消息并展示在消息展示区的信号槽
    connect(dataCenter,&DataCenter::receiveMessageDone,this,&MessageEditArea::addReceiveMessage);

    // 4.关联"发送图片"按钮信号槽
    connect(sendImageBtn,&QPushButton::clicked,this,&MessageEditArea::onClickSendImageBtn);

    // 5.关联"发送文件"按钮信号槽
    connect(sendFileBtn,&QPushButton::clicked,this,&MessageEditArea::onClickSendFileBtn);

    // 6.关联"发送语音"按钮信号槽
    connect(sendSpeechBtn,&QPushButton::pressed,this,&MessageEditArea::onPressSendSpeechBtn);
    connect(sendSpeechBtn,&QPushButton::released,this,&MessageEditArea::onReleaseSendSpeechBtn);
    SoundRecorder* soundRecoder = SoundRecorder::getInstance();
    connect(soundRecoder,&SoundRecorder::soundRecordDone,this,&MessageEditArea::sendSpeech);
}

void MessageEditArea::sendTextMessage()
{
    // 1.先确认目前是否有会话被选中
    DataCenter *dataCenter = DataCenter::getInstance();
    if(dataCenter->getCurrentChatSessionId().isEmpty())
    {
        LOG() << "当前未选中任何会话，不会发送任何消息！";
        // 上述日志，只是在开发阶段能看到，程序发布出去了，此时就无法看到了。
        // 因此需要让普通用户，也能看到 "提示"
        Toast::showMessage("当前未选中会话, 不发送任何消息!");
        return;
    }

    // 2.获取到输入框的内容，查看有没有内容，有内容可以发送，没有不做任何操作
    const QString& message = textEdit->toPlainText().trimmed();// 清除输入框前后两头的空白符
    if(message.isEmpty())
    {
        LOG() << "输入框为空!" ;
        return;
    }

    // 3.清空输入框中的内容
    textEdit->clear();

    // 4.通过网络将消息发送出去
    dataCenter->sendTextMessageAsync(dataCenter->getCurrentChatSessionId(),message);
}

void MessageEditArea::addSelfMessage(model::MessageType messageType, const QByteArray &content, const QString &extraInfo)
{
    DataCenter* dataCenter = DataCenter::getInstance();
    const QString& currentChatSessionId = dataCenter->getCurrentChatSessionId();

    // 1.构造一个message对象
    Message message = Message::makeMessage(messageType,currentChatSessionId,*dataCenter->getMyself(),content,extraInfo);
    dataCenter->addMessage(message);

    // 2.将消息显示到消息展示区
    MainWidget* mainWidget = MainWidget::getInstance();
    SessionShowArea* sessionShowArea = mainWidget->getSessionShowArea();
    sessionShowArea->addMessageItem(false,message);

    // 3.控制消息滚动区显示消息最底部
    sessionShowArea->scrollToEnd();

    // 4.发送信号通知会话列表，更新最后一条消息
    emit dataCenter->updateLastMessage(currentChatSessionId);
}

void MessageEditArea::addReceiveMessage(const Message &message)
{
    // 1.通过主界面获取消息展示区
    MainWidget* mainWidget = MainWidget::getInstance();
    SessionShowArea* sessionShowArea = mainWidget->getSessionShowArea();

    // 2.将消息添加到消息展示区
    sessionShowArea->addMessageItem(true,message);

    // 3.控制消息展示区滚动条滚动到最底部
    sessionShowArea->scrollToEnd();

    // 4.提示一个收到新消息

    Toast::showMessage("收到新消息!");
}

void MessageEditArea::onClickSendImageBtn()
{
    DataCenter* dataCenter = DataCenter::getInstance();
    // 1.判定当前是否选中会话
    if(dataCenter->getCurrentChatSessionId().isEmpty())
    {
        LOG() << "当前未选中会话！";
        Toast::showMessage("您尚未选择任何会话, 不能发送图片!");
        return;
    }

    // 2.打开文件对话框选择文件
    QString filter = "Image Files (*.jpg *.png *.jpeg)";
    QString filePaths = QFileDialog::getOpenFileName(this,"选择图片",QDir::homePath(),filter);
    if(filePaths.isEmpty())
    {
        LOG() << "用户已关闭文件对话框！";
        return;
    }

    // 3.读取图片内容
    QByteArray fileContent = readFiletoQByteArray(filePaths);

    // 4.发送网络请求
    dataCenter->sendImageMessageAsync(fileContent,dataCenter->getCurrentChatSessionId());
}

void MessageEditArea::onClickSendFileBtn()
{
    DataCenter* dataCenter = DataCenter::getInstance();
    // 1.判定此时是否已经选中会话
    if(dataCenter->getCurrentChatSessionId().isEmpty())
    {
        LOG() << "当前未选中会话！";
        Toast::showMessage("您尚未选择任何会话, 不能发送文件!");
        return;
    }

    // 2.打开文件对话框选择文件
    QString filter = "*";
    QString filePaths = QFileDialog::getOpenFileName(this,"选择文件",QDir::homePath(),filter);
    if(filePaths.isEmpty())
    {
        LOG() << "用户已关闭文件对话框！";
        return;
    }

    // 3.读取文件内容
    QByteArray content = readFiletoQByteArray(filePaths);

    // 4.获取文件名
    QFileInfo fileinfo(filePaths);
    const QString& fileName = fileinfo.fileName();

    // 5.发送网络请求
    dataCenter->sendFileMessageAsync(dataCenter->getCurrentChatSessionId(),fileName,content);
}

void MessageEditArea::onPressSendSpeechBtn()
{
    // 1.判定此时是否已经选中会话
    DataCenter* dataCenter = DataCenter::getInstance();
    if(dataCenter->getCurrentChatSessionId().isEmpty())
    {
        LOG() << "当前未选中会话!";
        return;
    }

    // 2.切换语音按钮图标
    sendSpeechBtn->setIcon(QIcon(":/resource/image/speech_active.png"));
    tipLabel->show();

    // 3.开始录音
    SoundRecorder* soundRecorder = SoundRecorder::getInstance();
    soundRecorder->startRecord();
    textEdit->hide();
}

void MessageEditArea::onReleaseSendSpeechBtn()
{
    // 1.判定此时是否已经选中会话
    DataCenter* dataCenter = DataCenter::getInstance();
    if(dataCenter->getCurrentChatSessionId().isEmpty())
    {
        LOG() << "当前未选中会话!";
        return;
    }

    // 2.切换语音按钮图标
    sendSpeechBtn->setIcon(QIcon(":/resource/image/speech.png"));
    tipLabel->hide();
    textEdit->show();

    // 3.停止录音
    SoundRecorder* soundRecorder = SoundRecorder::getInstance();
    soundRecorder->stopRecord();
}

void MessageEditArea::sendSpeech(const QString &filePath)
{
    DataCenter* dataCenter = DataCenter::getInstance();
    // 1.读取文件内容
    QByteArray content = readFiletoQByteArray(filePath);
    if(content.isEmpty())
    {
        LOG() << "语音文件为空!";
        return;
    }

    // 2.发送网络请求将消息发送出去
    dataCenter->sendSpeechMessageAsync(dataCenter->getCurrentChatSessionId(),content);
}








