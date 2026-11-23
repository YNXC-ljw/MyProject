#ifndef HISTORYMESSAGEWIDGET_H
#define HISTORYMESSAGEWIDGET_H

#include <QDialog>
#include <QWidget>
#include <QGridLayout>
#include <QRadioButton>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
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

////////////////////////////////////////////////////////////////
/// 展示图片历史消息
////////////////////////////////////////////////////////////////

class ImageButton : public QPushButton {
public:
    ImageButton(const QString& fileId, const QByteArray& content);

    void updateUI(const QString& fileId, const QByteArray& content);
private:
    QString fileId;
};
////////////////////////////////////////////////////////////////
/// 展示文件历史消息
////////////////////////////////////////////////////////////////

class FileLabel : public QLabel {
public:
    FileLabel(const QString& fileId, const QString& fileName);
    void updateUI(const QString& fileId, const QByteArray& content);
    void mousePressEvent(QMouseEvent* event) override;
private:
    QString fileId;
    QByteArray content;
    QString fileName;
    bool loadDone = false;
};
////////////////////////////////////////////////////////////////
/// 展示语音历史消息
////////////////////////////////////////////////////////////////

class SpeechLabel : public QLabel {
public:
    SpeechLabel(const QString& fileId);
    void updateUI(const QString& fileId, const QByteArray& content);
    void mousePressEvent(QMouseEvent* event);
private:
    QString fileId;
    QByteArray content;
    bool loadDone = false;
};
#endif // HISTORYMESSAGEWIDGET_H









