#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>

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

    // 初始化信号与槽
    void initSignalSlot();

    ///////////////////////////////////
    /// 窗口各部分所用对象
    ///////////////////////////////////
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

    // 搜索框
    QLineEdit* searchLine;
    // 添加好友按钮
    QPushButton* addFriendBtn;

    enum ActiveTab
    {
        SESSION_LIST,
        FRIEND_LIST,
        APPLY_LIST
    };
    ActiveTab activeTab = SESSION_LIST;

    // 标签按钮点击，更新按钮状态
    void switchTabToSession();
    void switchTabToFriend();
    void switchTabToApply();
    // 标签按钮点击，对应界面显示
    void loadSessionList();
    void loadFriendList();
    void loadApplyList();
};
#endif // MAINWIDGET_H
