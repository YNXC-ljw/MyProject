#ifndef SESSIONSHOWAREA_H
#define SESSIONSHOWAREA_H

#include <QScrollArea>
#include <QWidget>
#include <QLabel>
#include <QPushButton>

#include "model/data.h"

// .h 文件中不宜进行 using namespace xxxx
using model::Message;

////////////////////////////////////////////////////
/// 表示消息展示区
////////////////////////////////////////////////////
class SessionShowArea : public QScrollArea
{
    Q_OBJECT
public:
    SessionShowArea();

    // 添加消息体至消息展示区(尾插)
    void addMessageItem(bool isLeft,const Message& message);
    // 头插
    void addFrontMessageItem(bool isLeft,const Message& message);
    // 清空
    void clear();
    // 将滚动区滚动到最底下，显示最近一条消息
    void scrollToEnd();
private:
    QWidget* container;
};

////////////////////////////////////////////////////
/// 表示一个消息元素
/// 这个里面要能同时支持 文本消息 语音消息 图片消息 文件消息
/// 暂时只考虑文本消息，后续完善其他消息
////////////////////////////////////////////////////

class MessageItem : public QWidget
{
    Q_OBJECT
public:
    MessageItem(bool isLeft);
    // 通过 工厂模式 创建 MessageItem实例
    static MessageItem* makeMessageItem(bool isLeft,const Message& message);

    // 添加工厂函数
    static QWidget* makeTextMessageItem(bool isLeft,const QString& text);
    static QWidget* makeFileMessageItem(bool isLeft, const Message& message);
    static QWidget* makeImageMessageItem(bool isLeft,const QString& fileId,const QByteArray& content);
    static QWidget* makeSpeechMessageItem(bool isLeft,const Message& message);

private:
    // 此处的 isLeft 表示该Item是否属于"左侧消息"
    bool isLeft;
};

////////////////////////////////////////////////////
/// 创建类表示“文本消息”和“文件消息”正文部分
////////////////////////////////////////////////////
class MessageContentLabel : public QWidget{
    Q_OBJECT
public:
    MessageContentLabel(const QString& text,bool isLeft,model::MessageType messageType,const QString& fileId,
                const QByteArray& content);

    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;
    void updateUI(const QString& fileId,const QByteArray& content);
    void saveCurrentFile(const QByteArray& content);
    void playDone();
    void speechConvertToTextDone(const QString& fileId, const QString& text);
private:
    QLabel* label;
    bool isLeft;

    model::MessageType messageType;
    QString fileId;
    QByteArray content;
    QString fileName;

    bool getSingleFileDone = false; // 标记文件是否加载完毕
};
////////////////////////////////////////////////////
/// 创建类表示“图片消息”正文部分
////////////////////////////////////////////////////
class MessageImageLabel : public QWidget{
    Q_OBJECT
public:
    MessageImageLabel(const QString& fileId,const QByteArray& content,bool isLeft);

    void updateUI(const QString& fileId,const QByteArray& content);
    void paintEvent(QPaintEvent* event) override;
private:
    QPushButton* imageBtn;

    QString fileId; // 该图片在服务器上对应的文件 id
    QByteArray content; // 图片的二进制消息
    bool isLeft;
};

#endif // SESSIONSHOWAREA_H
