#include "widget.h"
#include "server.h"
#include "./ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    WebSocketServer* webSocketServer = WebSocketServer::getInstance();
    emit webSocketServer->sendTextResp();
}


void Widget::on_pushButton_2_clicked()
{
    WebSocketServer* webSocketServer = WebSocketServer::getInstance();
    emit webSocketServer->deleteFriendResp();
}


void Widget::on_pushButton_3_clicked()
{
    WebSocketServer* webSocketServer = WebSocketServer::getInstance();
    emit webSocketServer->addFriendApply();
}


void Widget::on_pushButton_4_clicked()
{
    WebSocketServer* webSocketServer = WebSocketServer::getInstance();
    emit webSocketServer->acceptFriendApply(true);
}


void Widget::on_pushButton_5_clicked()
{
    WebSocketServer* webSocketServer = WebSocketServer::getInstance();
    emit webSocketServer->rejectFriendApply(false);
}


void Widget::on_pushButton_6_clicked()
{
    WebSocketServer* webSocketServer = WebSocketServer::getInstance();
    emit webSocketServer->sendCreateSession();
}


void Widget::on_pushButton_7_clicked()
{
    WebSocketServer* webSocketServer = WebSocketServer::getInstance();
    emit webSocketServer->sendImageMessage();
}


void Widget::on_pushButton_8_clicked()
{
    WebSocketServer* webSocketServer = WebSocketServer::getInstance();
    emit webSocketServer->sendFileMessage();
}


void Widget::on_pushButton_9_clicked()
{
    WebSocketServer* webSocketServer = WebSocketServer::getInstance();
    emit webSocketServer->sendSpeechMessage();
}

