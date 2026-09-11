#ifndef SESSIONDETAILWIDGET_H
#define SESSIONDETAILWIDGET_H

#include <QDialog>
#include <QWidget>
#include <QLabel>

////////////////////////////////////////////////
/// 表示整个"单聊会话详情"窗口
////////////////////////////////////////////////
class SessionDetailWidget : public QDialog
{
    Q_OBJECT
public:
    SessionDetailWidget(QWidget* parent);

    QPushButton* deleteFriendBtn;
};

////////////////////////////////////////////////
/// 表示一个头像 + 一个名字组合控件
////////////////////////////////////////////////

class AvatarItem : public QWidget
{
    Q_OBJECT
public:
    AvatarItem(const QIcon& avatar,const QString& name);

    QPushButton* getAvatarBtn()
    {
        return avatarBtn;
    }
private:
    QPushButton* avatarBtn;
    QLabel* nameLabel;
};


#endif // SESSIONDETAILWIDGET_H
