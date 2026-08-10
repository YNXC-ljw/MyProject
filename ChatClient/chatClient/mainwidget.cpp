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
    this->setWindowIcon(QIcon(":/resource/image/logo.png"));
    QIcon icon(":/resource/image/logo.png");
    qDebug() << "图标是否为空？" << icon.isNull();          // 如果输出 true，说明资源没找到
    qDebug() << "可用尺寸：" << icon.availableSizes();     // 如果为空，也是没加载成功
}

MainWidget::~MainWidget()
{
    delete ui;
}
