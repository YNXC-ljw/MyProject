#ifndef SESSIONFRIENDAREA_H
#define SESSIONFRIENDAREA_H

#include <QWidget>
#include <QScrollArea>
#include <QLabel>

#include "model/data.h"

/////////////////////////////////////////
/// 滚动区域中Item的类型
/////////////////////////////////////////

enum ItemType
{
    SESSIONITEM_TYPE,
    FRIENDITEM_TYPE,
    APPLYITEM_TYPE
};

/////////////////////////////////////////
/// 整个滚动区的实现
/////////////////////////////////////////

class sessionFriendArea : public QScrollArea
{
    Q_OBJECT
public:
    explicit sessionFriendArea(QWidget *parent = nullptr);

    // 清理所有Item
    void clear();
    // 往container内部添加Item
    void addItem(ItemType itemType,const QString& id,const QIcon& avatar,const QString &name,const QString& text);

private:
    // 后续往container内部的layout中添加元素，就能触发QScrollArea的滚动效果
    QWidget* container;
signals:
};
/////////////////////////////////////////
/// 滚动区域中Item的实现
/////////////////////////////////////////
class SessionFriendItem : public QWidget
{
    Q_OBJECT
public:
    SessionFriendItem(QWidget* owner,const QIcon& avatar,const QString& name,const QString& text);

    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override; // 重写鼠标点击事件实现非按钮区域点击触发事件
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;

    void select();

    virtual void active();

private:
    // 该指针指向滑动块sessionFriendArea
    QWidget* owner;

    // 该变量标记当前对话Item是否被选中
    bool selected = false;
protected:
    // 让该对象能够让子类访问
    QLabel* messageLabel;
};

/////////////////////////////////////////
/// 会话Item的实现
/////////////////////////////////////////

class SessionItem : public SessionFriendItem
{
    Q_OBJECT
public:
    SessionItem(QWidget* owner,const QString& chatSessionId,const QIcon& avatar,const QString& name,
                const QString &lastMessage);
    void active() override;
private:
    // 当前会话ID
    QString chatSessionId;
};

/// /////////////////////////////////////
/// 好友Item的实现
/////////////////////////////////////////

class FriendItem : public SessionFriendItem
{
    Q_OBJECT
public:
    FriendItem(QWidget* owner,const QString& userId,const QIcon& avatar,const QString& name,
               const QString& description);
    void active() override;

private:
    QString userId;
};

/////////////////////////////////////////
/// 申请列表Item的实现
/////////////////////////////////////////

class ApplyItem : public SessionFriendItem
{
    Q_OBJECT
public:
    ApplyItem(QWidget* owner,const QString& userId,const QIcon& avatar,const QString& name);
    void active() override;

private:
    QString userId;
};

#endif // SESSIONFRIENDAREA_H
