#include "addfrienddialog.h"

#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>

AddFriendDialog::AddFriendDialog(QWidget* parent) :QDialog(parent)
{
    // 1.设置基本属性
    this->setFixedSize(500,500);
    this->setWindowTitle("添加好友");
    this->setStyleSheet("QDialog { background-color: rgb(255,255,255); }");
    this->setWindowIcon(QIcon(":/resource/image/logo.png"));
    this->setAttribute(Qt::WA_DeleteOnClose);

    // 2.创建布局管理器
    QGridLayout* glayout = new QGridLayout();
    glayout->setSpacing(10);
    glayout->setContentsMargins(20,20,20,0);
    this->setLayout(glayout);

    // 3.创建搜索框
    QLineEdit* searchEdit = new QLineEdit();
    searchEdit->setFixedHeight(50);
    searchEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    QString style = "QLineEdit { border: none; border-radius: 10px; font-size: 16px; background-color: rgb(240,240,240); }";
    style += "QLineEdit { padding-left: 5px; }";
    searchEdit->setStyleSheet(style);
    searchEdit->setPlaceholderText("按手机号/用户序号/用户昵称搜索");
    glayout->addWidget(searchEdit,0,0,1,8);

    // 4. 创建搜索按钮
    QPushButton* searchBtn = new QPushButton();
    searchBtn->setFixedSize(50, 50);
    searchBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    searchBtn->setIconSize(QSize(30, 30));
    searchBtn->setIcon(QIcon(":/resource/image/search.png"));
    QString btnStyle = "QPushButton { border: none; background-color: rgb(240, 240, 240); border-radius: 10px; }";
    btnStyle += "QPushButton:hover { background-color: rgb(220, 220, 220); } QPushButton:pressed { background-color: rgb(200, 200, 200); }";
    searchBtn->setStyleSheet(btnStyle);
    glayout->addWidget(searchBtn,0,8,1,1);

    // 5.添加结果显示区域
    initResultArea();
}

void AddFriendDialog::initResultArea()
{

}








