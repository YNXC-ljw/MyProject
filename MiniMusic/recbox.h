#ifndef RECBOX_H
#define RECBOX_H

#include <QWidget>
#include<QJsonArray>

namespace Ui {
class RecBox;
}

class RecBox : public QWidget
{
    Q_OBJECT

public:
    explicit RecBox(QWidget *parent = nullptr);
    ~RecBox();

    void initRecBoxUi(QJsonArray data,int row);


private:
    void createRecBoxItem();
private:
    Ui::RecBox *ui;
    int row;
    int col;

    QJsonArray imageList;
};

#endif // RECBOX_H
