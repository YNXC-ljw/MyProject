#include "groupsessiondetailwidget.h"

#include "sessiondetailwidget.h"
#include "debug.h"

#include <QVBoxLayout>
#include <QScrollArea>
#include <QScrollBar>
#include <QPushButton>

GroupSessionDetailWidget::GroupSessionDetailWidget(QWidget* parent) :QDialog(parent)
{
    // 1.设置窗口基本属性
    this->setFixedSize(410,600);
    this->setWindowTitle("群聊详情");
    this->setWindowIcon(QIcon(":/resource/image/logo.png"));
    this->setStyleSheet("QDialog { background-color: rgb(255,255,255); }");
    this->setAttribute(Qt::WA_DeleteOnClose); // 关闭窗口时释放资源

    // 2.创建布局管理器
    QVBoxLayout* vlayout = new QVBoxLayout();
    vlayout->setSpacing(0);
    vlayout->setContentsMargins(0,0,0,0);
    this->setLayout(vlayout);

    // 3.创建滚动区域
    // 3.1 创建QScrollArea对象
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width: 2px; background-color: rgb(255,255,255); }");
    scrollArea->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal { height: 0px; }");
    scrollArea->setFixedSize(310,350);
    scrollArea->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    // 3.2 创建QWidget对象
    QWidget* container = new QWidget();
    scrollArea->setWidget(container);

    // 3.3 给container里面再添加一个网格布局
    QGridLayout* glayout = new QGridLayout();
    glayout->setContentsMargins(0,0,0,0);
    glayout->setSpacing(0);
    glayout->setAlignment(Qt::AlignTop | Qt::AlignLeft); // 控件都按左上对齐
    container->setLayout(glayout);

    // 3.4 把布局管理器添加到滚动区域
    vlayout->addWidget(scrollArea);

    // 4. 添加 "添加按钮"
    AvatarItem* addBtn = new AvatarItem(QIcon(":/resource/image/cross.png"), "添加");
    glayout->addWidget(addBtn, 0, 0);

    // 5. 添加 "群聊名称"
    QLabel* groupNameTag = new QLabel();
    groupNameTag->setText("群聊名称");
    groupNameTag->setStyleSheet("QLabel {font-weight: 700;}");
    vlayout->addWidget(groupNameTag);

    // 6. 添加 真实的群聊名字 和 修改按钮
    // 6.1 创建水平布局
    QHBoxLayout* hlayout = new QHBoxLayout();
    hlayout->setSpacing(0);
    hlayout->setContentsMargins(0, 0, 0, 0);
    vlayout->addLayout(hlayout);

    // 6.2 创建真实群聊名字的 label
    QLabel* groupNameLabel = new QLabel();
    groupNameTag->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
    hlayout->addWidget(groupNameLabel);



// 此处构造假的数据用来测试界面
#if TEST_UI
    groupNameLabel->setText("人类吃喝行为研究小组");
#endif

}















