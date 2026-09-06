#include "sessiondetailwidget.h"

#include <QPushButton>
#include <QVBoxLayout>

////////////////////////////////////////////////
/// 表示整个"单聊会话详情"窗口
////////////////////////////////////////////////

SessionDetailWidget::SessionDetailWidget(QWidget* parent)
    :QDialog(parent)
{
    // 1.设置一些基本属性
    this->setWindowTitle("会话详情");
    this->setFixedSize(300,200);
    this->setWindowIcon(QIcon(":/resource/image/logo.png"));
    this->setAttribute(Qt::WA_DeleteOnClose);
}

////////////////////////////////////////////////
/// 表示一个头像 + 一个名字组合控件
////////////////////////////////////////////////

AvatarItem::AvatarItem(const QIcon &avatar, const QString &name)
{
    // 1.设置自身基本属性
    this->setFixedSize(120,80);

    // 2.创建垂直布局管理器
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    this->setLayout(layout);

    // 3.创建头像按钮
    avatarBtn = new QPushButton();
    avatarBtn->setFixedSize(45,45);
    avatarBtn->setIconSize(QSize(45,45));
    avatarBtn->setIcon(avatar);
    avatarBtn->setStyleSheet("QPushButton { border: none; }");

    // 4. 创建名字
    nameLabel = new QLabel();
    nameLabel->setText(name);
    QFont font("微软雅黑", 12);
    nameLabel->setFont(font);
    nameLabel->setAlignment(Qt::AlignCenter);

    // 5. 对名字做 "截断操作"
    const int MAX_WIDTH = 65;
    QFontMetrics metrics(font);
    int totalWidth = metrics.horizontalAdvance(name);
    if (totalWidth >= MAX_WIDTH) {
        // 需要截断
        QString tail = "...";
        int tailWidth = metrics.horizontalAdvance(tail);
        int availableWidth = MAX_WIDTH - tailWidth;
        int availableSize = name.size() * ((double)availableWidth / totalWidth);
        QString newName = name.left(availableSize);
        nameLabel->setText(newName + tail);
    }

    // 6.将控件添加到布局管理器中
    layout->addWidget(avatarBtn);
    layout->addWidget(nameLabel);

}














