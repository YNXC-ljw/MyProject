#include "messageeditarea.h"

#include <QVBoxLayout>
#include <QScrollBar>
#include <QHBoxLayout>

MessageEditArea::MessageEditArea(QWidget *parent)
    : QWidget{parent}
{
    // 1.设置必要属性
    this->setFixedHeight(200);
    this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    //this->setStyleSheet("border-radius: 10px; background-color: rgb(235,235,235);");
    
    // 2.创建垂直方向布局管理器
    QVBoxLayout* vlayout = new QVBoxLayout();
    vlayout->setSpacing(0);
    vlayout->setContentsMargins(10,0,10,10);
    this->setLayout(vlayout);

    // 5. 添加多行编辑框
    textEdit = new QPlainTextEdit();
    textEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    textEdit->setStyleSheet("QPlainTextEdit { border: none; background-color: transparent; font-size: 14px; padding: 10px;}");
    textEdit->verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width: 2px; background-color: rgb(45, 45, 45);}");
    vlayout->addWidget(textEdit);

    // 3.创建水平方向布局管理器
    QHBoxLayout* hlayout = new QHBoxLayout();
    hlayout->setSpacing(0);
    hlayout->setContentsMargins(10,0,0,10);
    hlayout->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
    vlayout->addLayout(hlayout);

    // 4.创建按钮，将其放入水平布局管理器中
    QString btnStyle = "QPushButton {background-color: rgb(245,245,245);} QPushButton:pressed {background-color: rgb(255,255,255);}";
    sendImageBtn = new QPushButton();
    sendImageBtn->setFixedSize(35,35);
    sendImageBtn->setIconSize(QSize(25,25));
    sendImageBtn->setIcon(QIcon(":/resource/image/image.png"));
    sendImageBtn->setStyleSheet(btnStyle);
    hlayout->addWidget(sendImageBtn);

    sendFileBtn = new QPushButton();
    sendFileBtn->setFixedSize(35,35);
    sendFileBtn->setIconSize(QSize(25,25));
    sendFileBtn->setIcon(QIcon(":/resource/image/file.png"));
    sendFileBtn->setStyleSheet(btnStyle);
    hlayout->addWidget(sendFileBtn);

    sendSpeechBtn = new QPushButton();
    sendSpeechBtn->setFixedSize(35,35);
    sendSpeechBtn->setIconSize(QSize(25,25));
    sendSpeechBtn->setIcon(QIcon(":/resource/image/speech.png"));
    sendSpeechBtn->setStyleSheet(btnStyle);
    hlayout->addWidget(sendSpeechBtn);

    showHistoryBtn = new QPushButton();
    showHistoryBtn->setFixedSize(35,35);
    showHistoryBtn->setIconSize(QSize(25,25));
    showHistoryBtn->setIcon(QIcon(":/resource/image/history.png"));
    showHistoryBtn->setStyleSheet(btnStyle);
    hlayout->addWidget(showHistoryBtn);

    hlayout->addStretch();

    // 6. 添加发送文本消息的按钮
    sendTextBtn = new QPushButton();
    sendTextBtn->setText("发送");
    sendTextBtn->setFixedSize(120, 40);
    QString style = "QPushButton { font-size: 16px; color: rgb(7, 193, 96); border: none; background-color: rgb(233, 233, 233);border-radius: 10px;}";
    style += "QPushButton:hover { background-color: rgb(210, 210, 210); }";
    style += "QPushButton:pressed { background-color: rgb(200, 200, 200); }";
    sendTextBtn->setStyleSheet(style);
    hlayout->addWidget(sendTextBtn);

}








