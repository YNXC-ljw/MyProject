#include "loginwidget.h"

#include <QGridLayout>
#include <QLabel>

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
    layout->setContentsMargins(0,0,0,0);
    this->setLayout(layout);

    // 3.创建标题
    QLabel* titleLabel = new QLabel();
    titleLabel->setText("登录");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setFixedHeight(50);

}
