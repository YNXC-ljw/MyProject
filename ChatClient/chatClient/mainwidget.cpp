#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget* MainWidget::instance = nullptr;

MainWidget *MainWidget::getInstance()
{
    if(instance == nullptr)
    {
        // 此处不传入参数，表示以桌面为父窗口
        instance = new MainWidget();
    }
    return instance;
}

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    this->setWindowTitle("我的聊天桌面");

}

MainWidget::~MainWidget()
{
    delete ui;
}
