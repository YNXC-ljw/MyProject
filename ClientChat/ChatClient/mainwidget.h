#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWidget;
}
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

private:
    static MainWidget* instance; // 懒汉式
    // 对于单例模式来说，最关键的不是创建实例，而是禁止别人创建实例
    MainWidget(QWidget *parent = nullptr);

public:
    static MainWidget* getInstance();

    ~MainWidget() override;

private:
    Ui::MainWidget *ui;

    ///////////////////////////////////
    /// 初始化窗口布局
    ///////////////////////////////////
    void initMainWindow();
    void initLeftWindow();
    void initMidWindow();
    void initRightWindow();
    // 窗口左侧区域
    QWidget* windowLeft;
    // 窗口中间区域
    QWidget* windowMid;
    // 窗口右侧区域
    QWidget* windowRight;

    // 用户头像
    QPushButton* userAvatar;
    // 会话标签页按钮
    QPushButton* sessionTabBtn;
    // 好友标签页按钮
    QPushButton* friendTabBtn;
    // 好友申请标签页按钮
    QPushButton* applyTabBtn;
};
#endif // MAINWIDGET_H
