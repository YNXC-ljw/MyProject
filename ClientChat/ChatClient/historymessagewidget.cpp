#include "historymessagewidget.h"

#include <QGridLayout>
#include <QRadioButton>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QDateTimeEdit>

HistoryMessageWidget::HistoryMessageWidget(QWidget* parent) :QDialog(parent)
{
    // 1.设置窗口基本属性
    this->setFixedSize(600,600);
    this->setWindowTitle("历史消息");
    this->setWindowIcon(QIcon(":/resource/image/logo.png"));
    this->setStyleSheet("QWidget { background-color: rgb(255,255,255);}");
    this->setAttribute(Qt::WA_DeleteOnClose);

    // 2.创建布局管理器
    QGridLayout* layout = new QGridLayout();
    layout->setSpacing(10);
    layout->setContentsMargins(30,0,30,0);
    this->setLayout(layout);

    // 3.创建单选按钮
    QRadioButton* keyRadioBtn = new QRadioButton();
    QRadioButton* timeRadioBtn = new QRadioButton();
    keyRadioBtn->setText("按关键词搜索");
    timeRadioBtn->setText("按时间搜索");
    layout->addWidget(keyRadioBtn,0,0,1,2);
    layout->addWidget(timeRadioBtn,0,2,1,2);

    // 4.创建搜索框
    QLineEdit* searchEdit = new QLineEdit();
    searchEdit->setFixedHeight(50);
    searchEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    searchEdit->setPlaceholderText("请输入搜索的关键词");
    searchEdit->setStyleSheet("QLineEdit { border:none; background-color: rgb(240,240,240); font-size: 16px; border-radius: 10px;padding-left: 10px;}");
    layout->addWidget(searchEdit,1,0,1,8);

    // 5.创建搜索按钮
    QPushButton* searchBtn = new QPushButton();
    searchBtn->setFixedSize(50,50);
    searchBtn->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    searchBtn->setIconSize(QSize(30,30));
    searchBtn->setIcon(QIcon(":/resource/image/search.png"));
    QString style = "QPushButton { border:none; border-radius: 5px; background-color: rgb(240,240,240); }";
    style += "QPushButton:pressed { backgorund-color: rgb(220,220,220);}";
    searchBtn->setStyleSheet(style);
    layout->addWidget(searchBtn,1,8,1,1);

    // 6.创建时间相关的控件
    // 6．创建时间相关的部分控件，初始情况下要隐藏
    QLabel* begTag = new QLabel();
    begTag->setText("开始时间");
    QLabel* endTag = new QLabel();
    endTag->setText("结束时间");
    QDateTimeEdit* begTimeEdit = new QDateTimeEdit();
    QDateTimeEdit* endTimeEdit = new QDateTimeEdit();
    begTimeEdit->setFixedHeight(50);
    endTimeEdit->setFixedHeight(50);
    begTag->hide();
    endTag->hide();
    begTimeEdit->hide();
    endTimeEdit->hide();

}





