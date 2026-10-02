#ifndef BTFORM_H
#define BTFORM_H

#include <QWidget>
#include<QPropertyAnimation>

namespace Ui {
class BtForm;
}

class BtForm : public QWidget
{
    Q_OBJECT

public:
    explicit BtForm(QWidget *parent = nullptr);
    ~BtForm();

    void setAnimation();

    void setIconAndText(const QString& btIcon,const QString& btText,int pageId);

    int getPageId() const;

    void showAnimal(bool isShow);
    void clearBackground();

    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;

signals:
    void btClicked(int Id); // 鼠标点击事件发送信号

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::BtForm *ui;
    int pageId;
    QPropertyAnimation* line1Animation;
    QPropertyAnimation* line2Animation;
    QPropertyAnimation* line3Animation;
    QPropertyAnimation* line4Animation;

    bool isSelected = false; // 标记此时该是否被选中
};

#endif // BTFORM_H
