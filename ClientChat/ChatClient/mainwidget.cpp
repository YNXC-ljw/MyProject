#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QHBoxLayout>

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

    this->setGeometry(400,200,880,600);

    this->setWindowTitle("我的聊天桌面");
    this->setWindowIcon(QIcon(":/resource/image/logo.png"));

    // 初始化主窗口
    initMainWindow();
    // 初始化窗口左侧
    initLeftWindow();
    // 初始化窗口中间
    initMidWindow();
    // 初始化窗口右侧
    initRightWindow();
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::initMainWindow()
{
    QHBoxLayout* layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);   // 去掉边距，更干净
    layout->setSpacing(0);                    // 去掉控件间距
    this->setLayout(layout);

    windowLeft = new QWidget();
    windowMid = new QWidget();
    windowRight = new QWidget();

    windowLeft->setFixedWidth(70);
    windowMid->setFixedWidth(200);
    windowRight->setMinimumWidth(600);

    windowLeft->setStyleSheet("QWidget { background-color: rgb(224,224,224); }");
    windowMid->setStyleSheet("QWidget { background-color: rgb(238,238,240); }");
    windowRight->setStyleSheet("QWidget { background-color: rgb(250,250,250); }");

    layout->addWidget(windowLeft);
    layout->addWidget(windowMid);
    layout->addWidget(windowRight);

}

void MainWidget::initLeftWindow()
{

}

void MainWidget::initMidWindow()
{

}

void MainWidget::initRightWindow()
{

}

