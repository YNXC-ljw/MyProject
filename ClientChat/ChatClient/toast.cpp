#include "toast.h"

#include <QApplication>
#include <QScreen>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>

Toast::Toast(const QString &text)
{
    // 1.设置窗口基本属性
    this->setFixedSize(800,80);
    this->setWindowTitle("新消息通知");
    this->setWindowIcon(QIcon(":/resource/image/logo.png"));
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setStyleSheet("QDialog { background-color: rgb(255,255,255); border: none; border-radius: 10px; }");
    // 去掉窗口标题栏
    this->setWindowFlags(Qt::FramelessWindowHint);

    // 2.设置窗口所在位置
    QScreen* screen = QApplication::primaryScreen();
    int width = screen->size().width();
    int height = screen->size().height();
    int x = (width - this->width()) / 2;
    int y = height - this->height() - 900; // 此处的 100 是窗口底边距离屏幕底边的间隔
    this->move(x, y);

    // 3. 添加一个布局管理器
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(layout);

    // 4. 创建显示文本的 Label
    QLabel* label = new QLabel();
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("QLabel { font-size: 32px; }");
    label->setText(text);
    layout->addWidget(label);

    // 5.两秒后自动关闭
    QTimer* timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,[=](){
        timer->stop();
        // 关闭窗口
        this->close();
    });
    timer->start(2000);
}

void Toast::showMessage(const QString &text)
{
    Toast* toast = new Toast(text);
    toast->show();
}









