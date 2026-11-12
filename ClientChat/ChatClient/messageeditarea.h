#ifndef MESSAGEEDITAREA_H
#define MESSAGEEDITAREA_H

#include <QWidget>
#include <QPushButton>
#include <QPlainTextEdit>
#include "model/data.h"

class MessageEditArea : public QWidget
{
    Q_OBJECT
public:
    explicit MessageEditArea(QWidget *parent = nullptr);
    void initSignalSlot();

    void sendTextMessage();
    void addSelfMessage(model::MessageType messageType, const QByteArray& message, const QString& extraInfo);

    void addReceiveMessage(const model::Message& message);
private:
    QPushButton* sendImageBtn;
    QPushButton* sendFileBtn;
    QPushButton* sendSpeechBtn;
    QPushButton* showHistoryBtn;
    QPlainTextEdit* textEdit;
    QPushButton* sendTextBtn;

signals:
};

#endif // MESSAGEEDITAREA_H
