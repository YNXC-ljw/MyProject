#include "mainwidget.h"
#include "model/data.h"
#include "loginwidget.h"
#include "debug.h"
#include "model/datacenter.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

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
