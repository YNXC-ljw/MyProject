#ifndef SESSIONSHOWAREA_H
#define SESSIONSHOWAREA_H

#include <QScrollArea>
#include <QWidget>
#include <QLabel>


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
    static QWidget* makeFileMessageItem();
    static QWidget* makeImageMessageItem();
    static QWidget* makeSpeechMessageItem();

private:
    // 此处的 isLeft 表示该Item是否属于"左侧消息"
    bool isLeft;
};

////////////////////////////////////////////////////
/// 创建类表示“文本消息”正文部分
////////////////////////////////////////////////////
class MessageContentLabel : public QWidget{
    Q_OBJECT
public:
    MessageContentLabel(const QString& text,bool isLeft);

    void paintEvent(QPaintEvent* event) override;
private:
    QLabel* label;
    bool isLeft;
};

#endif // SESSIONSHOWAREA_H
