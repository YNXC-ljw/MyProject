#ifndef CHOOSEFRIENDDIALOG_H
#define CHOOSEFRIENDDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QCheckBox>

/////////////////////////////////////////////
/// 选择好友窗口中的一个 元素/好友项
/////////////////////////////////////////////

class ChooseFriendItem : public QWidget
{
    Q_OBJECT
public:
    ChooseFriendItem(const QIcon& avatar,const QString& name,bool checked);

    void paintEvent(QPaintEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
private:
    bool isHover;

    QPushButton *avatarBtn;
    QLabel* nameLabel;
    QCheckBox* checkBox;
    bool checked;
};

/////////////////////////////////////////////
/// 选择好友的窗口
/////////////////////////////////////////////

class ChooseFriendDialog : public QDialog
{
    Q_OBJECT
public:
    ChooseFriendDialog(QWidget* parent);

    void initLeft(QHBoxLayout* layout);
    void initRight(QHBoxLayout* layout);

    void addFriendItem(const QIcon& avatar,const QString& name,bool checked);
private:
    // 保存左侧全部好友列表的 QWidget
    QWidget* selectedContainer;
    // 保存右侧选中好友列表的 QWidget
    QWidget* totalContainer;
};

#endif // CHOOSEFRIENDDIALOG_H
