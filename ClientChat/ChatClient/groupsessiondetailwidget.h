#ifndef GROUPSESSIONDETAILWIDGET_H
#define GROUPSESSIONDETAILWIDGET_H

#include <QDialog>
#include <QWidget>
#include <QGridLayout>

class AvatarItem;

class GroupSessionDetailWidget : public QDialog
{
    Q_OBJECT
public:
    GroupSessionDetailWidget(QWidget* parent);

    void addMenber(AvatarItem* avatarItem);
private:
    QGridLayout* glayout;

    // 表示当前要添加的 AvatarItem 所处的行和列
    // 由于整个界面上存在 "+" 这个按钮，占据了 (0, 0) 这个位置，所以第一个 AvatarItem 就要从 (0, 1) 开始了
    int curCol = 1;
    int curRow = 0;
};

#endif // GROUPSESSIONDETAILWIDGET_H
