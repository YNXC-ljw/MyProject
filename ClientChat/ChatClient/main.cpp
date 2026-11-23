#include "mainwidget.h"
#include "model/data.h"
#include "loginwidget.h"
#include "debug.h"
#include "model/datacenter.h"

#include <QApplication>

FILE* output = nullptr;

void msgHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
    (void) type;
    (void) context;
    const QByteArray& log = msg.toUtf8();
    fprintf(output, "%s\n", log.constData());
    fflush(output);  // 确保数据落入硬盘
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#if DEPOLY
    output = fopen("./log.txt","a");
    qInstallMessageHandler(msgHandler);
#endif

#if TEST_SKIP_LOGIN
    MainWidget* w = MainWidget::getInstance();
    w->show();
#else
    LoginWidget* loginWidget = new LoginWidget();
    loginWidget->show();
#endif

#if TEST_NETWORK
    // network::NetClient netClient(nullptr);
    // netClient.ping();
    model::DataCenter* dataCenter = model::DataCenter::getInstance();
    dataCenter->ping();
#endif

    return QApplication::exec();
}
