#include "mainwidget.h"
#include "model/data.h"
#include "loginwidget.h"
#include "debug.h"

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
    return QApplication::exec();
}
