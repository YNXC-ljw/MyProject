#include "widget.h"
#include "server.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    HttpServer* httpServer = HttpServer::getInstance();
    if(!httpServer->init())
    {
        qDebug() << "HTTP服务器启动失败!";
        return 1;
    }
    qDebug() << "HTTP服务器启动成功！";

    WebSocketServer* webSocketServer = WebSocketServer::getInstance();
    if(!webSocketServer->init())
    {
        qDebug() << "websocket 服务器启动失败!";
        return 1;
    }
    qDebug() << "websocket 服务器启动成功!";

    Widget w;
    w.show();
    return QApplication::exec();
}
