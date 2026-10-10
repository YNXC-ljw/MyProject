#ifndef ADDFRIENDDIALOG_H
#define ADDFRIENDDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QGridLayout>

#include "model/data.h"

using model::UserInfo;

//////////////////////////////////////////////////////////
/// 表示一个好友搜索的结果
//////////////////////////////////////////////////////////
class FriendResultItem : public QWidget
{
    Q_OBJECT
public:
    FriendResultItem(const UserInfo& userInfo);
private:
    const UserInfo& userInfo;
};

//////////////////////////////////////////////////////////
/// 整个搜索好友的窗口
//////////////////////////////////////////////////////////
class AddFriendDialog : public QDialog
{
    Q_OBJECT
public:
    AddFriendDialog(QWidget* parent);

    // 初始化结果显示区域
    void initResultArea();

    void addResult(const UserInfo& userInfo);
    void clear();

private:
    QGridLayout* glayout; // 整个窗口总的网格布局

    QWidget* resultContainer; // 放搜索结果的QWidget
};

#endif // ADDFRIENDDIALOG_H
