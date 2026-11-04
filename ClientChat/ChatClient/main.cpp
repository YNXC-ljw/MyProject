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

    // 临时的测试代码
    model::DataCenter* dataCenter = model::DataCenter::getInstance();
    dataCenter->initDataFile();
    dataCenter->saveDataToFile();

    return QApplication::exec();
}
