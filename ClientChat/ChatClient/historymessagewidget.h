#ifndef HISTORYMESSAGEWIDGET_H
#define HISTORYMESSAGEWIDGET_H

#include <QDialog>
#include <QWidget>
#include <QGridLayout>
#include <QRadioButton>
#include <QLineEdit>
#include "model/data.h"

using model::Message;

///////////////////////////////////////////////////////////
/// 表示一个历史消息元素
///////////////////////////////////////////////////////////

class HistoryItem : public QWidget
{
    Q_OBJECT
public:
    HistoryItem(){};

    static HistoryItem* makeHistoryItem(const Message& message);
};

///////////////////////////////////////////////////////////
/// 表示整个历史消息搜索框
///////////////////////////////////////////////////////////
class HistoryMessageWidget : public QDialog
{
    Q_OBJECT
public:
    HistoryMessageWidget(QWidget* parent);
    void addHistoryMessage(const Message& message);
    void clear();

    void onClickSearchBtn();
    void getSearchMessageDone();
private:
    QWidget* container; // 存储历史消息元素的区域
    QRadioButton* keyRadioBtn;
    QRadioButton* timeRadioBtn;
    QLineEdit* searchEdit;

    QDateTimeEdit* begTimeEdit;
    QDateTimeEdit* endTimeEdit;

    void initScrollArea(QGridLayout* layout);
};

#endif // HISTORYMESSAGEWIDGET_H
