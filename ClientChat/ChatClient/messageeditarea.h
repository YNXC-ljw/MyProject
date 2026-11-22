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

public slots:
    void onClickSendImageBtn();
    void onClickSendFileBtn();
    void onPressSendSpeechBtn();
    void onReleaseSendSpeechBtn();
    void sendSpeech(const QString& filePath);
private:
    QPushButton* sendImageBtn;
    QPushButton* sendFileBtn;
    QPushButton* sendSpeechBtn;
    QPushButton* showHistoryBtn;
    QPlainTextEdit* textEdit;
    QPushButton* sendTextBtn;

    QLabel* tipLabel;
signals:
};

#endif // MESSAGEEDITAREA_H
