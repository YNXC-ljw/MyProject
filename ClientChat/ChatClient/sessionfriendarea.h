#ifndef SESSIONFRIENDAREA_H
#define SESSIONFRIENDAREA_H

#include <QWidget>
#include <QScrollArea>

class sessionFriendArea : public QScrollArea
{
    Q_OBJECT
public:
    explicit sessionFriendArea(QWidget *parent = nullptr);

private:
    // 后续往container内部的layout中添加元素，就能触发QScrollArea的滚动效果
    QWidget* container;
signals:
};

#endif // SESSIONFRIENDAREA_H
