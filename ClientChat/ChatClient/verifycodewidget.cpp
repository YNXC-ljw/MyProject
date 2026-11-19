#include "verifycodewidget.h"
#include "model/data.h"
#include <QPainter>

VerifyCodeWidget::VerifyCodeWidget(QWidget *parent)
    : QWidget{parent},randomGenerator(model::getTime()) // 传入随机种子
{
    verifyCode = generatorVerifyCode();
}

QString VerifyCodeWidget::generatorVerifyCode()
{
    QString result;
    for(int i = 0;i < 4;i++)
    {
        int init = 'A';
        init += randomGenerator.generate() % 26;
        result += static_cast<QChar>(init);
    }
    return result;
}

bool VerifyCodeWidget::cheakVerifyCode(const QString &verifyCode)
{
    // 比较验证码时需忽略大小写
    return this->verifyCode.compare(verifyCode,Qt::CaseInsensitive) == 0;
}

void VerifyCodeWidget::refrushVerifyCode()
{
    // 每调用一次该函数就重新生成一次验证码
    verifyCode = generatorVerifyCode();

    // 通过该函数触发重绘事件
    this->update();
}

void VerifyCodeWidget::paintEvent(QPaintEvent *event)
{
    (void)event;
    QRect rc = this->rect();
    int w = rc.width();
    int h = rc.height();

    QPainter painter(this);
    QPen pen;
    QFont font("楷体",25,QFont::Bold,true);
    painter.setFont(font);

    // 随机噪点
    for(int i = 0; i < 50; i++)
    {
        pen = QPen(QColor(randomGenerator.generate() % 256,
                          randomGenerator.generate() % 256,
                          randomGenerator.generate() % 256));
        pen.setWidth(1);
        painter.setPen(pen);
        painter.drawPoint(randomGenerator.generate() % w,
                          randomGenerator.generate() % h);
    }

    // 随机干扰线
    for(int i = 0; i < 5; i++)
    {
        pen = QPen(QColor(randomGenerator.generate() % 256,
                          randomGenerator.generate() % 256,
                          randomGenerator.generate() % 256));
        pen.setWidth(1);
        painter.setPen(pen);
        painter.drawLine(randomGenerator.generate() % w,
                         randomGenerator.generate() % h,
                         randomGenerator.generate() % w,
                         randomGenerator.generate() % h);
    }

    // 绘制验证码文字（重点修复drawText）
    int charWidth = 100/4;
    for(int i = 0; i < verifyCode.size(); i++)
    {
        pen = QPen(QColor(randomGenerator.generate() % 200,
                          randomGenerator.generate() % 200,
                          randomGenerator.generate() % 200));
        pen.setWidth(2);
        painter.setPen(pen);
        int x = i * charWidth;
        // y只小幅上下浮动，范围很小，不会飘得乱七八糟
        int y = 4 + randomGenerator.generate() % 6;
        painter.drawText(x, y, charWidth, 20, Qt::AlignCenter, verifyCode[i]);
    }
}

void VerifyCodeWidget::mousePressEvent(QMouseEvent *event)
{
    (void) event;
    this->refrushVerifyCode();
}
