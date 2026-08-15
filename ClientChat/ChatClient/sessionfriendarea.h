#ifndef SESSIONFRIENDAREA_H
#define SESSIONFRIENDAREA_H

#include <QWidget>
#include <QScrollArea>

class sessionFriendArea : public QScrollArea
{
    Q_OBJECT
public:
    explicit sessionFriendArea(QWidget *parent = nullptr);

    // 清理所有Item
    void clear();
    // 往container内部添加Item
    void addItem(const QIcon& avatar,const QString &name,const QString& text);

private:
    // 后续往container内部的layout中添加元素，就能触发QScrollArea的滚动效果
    QWidget* container;
signals:
};
class SessionFriendItem : public QWidget
{
    Q_OBJECT
public:
    SessionFriendItem(QWidget* owner,const QIcon& avatar,const QString& name,const QString& text);
private:
    // 该指针指向滑动块sessionFriendArea
    QWidget* owner;
    // 该变量标记当前对话Item是否被选中
    bool selected = false;
};

#endif // SESSIONFRIENDAREA_H
