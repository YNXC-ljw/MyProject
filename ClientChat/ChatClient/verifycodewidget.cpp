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
    return this->verifyCode.compare(verifyCode,Qt::CaseSensitive) == 0;
}

void VerifyCodeWidget::refrushVerifyCode()
{
    // 每调用一次该函数就重新生成一次验证码
    generatorVerifyCode();

    // 通过该函数触发重绘事件
    this->update();
}

void VerifyCodeWidget::paintEvent(QPaintEvent *event)
{
    (void)event;
    const int width = 180;
    const int height = 80;

    QPainter painter(this);
    QPen pen;
    QFont font("楷体",25,QFont::Bold,true);
    painter.setFont(font);

    // 随机噪点
    for(int i = 0; i < 100; i++)
    {
        pen = QPen(QColor(randomGenerator.generate() % 256,randomGenerator.generate() % 256,randomGenerator.generate() % 256));
        pen.setWidth(2); // 加粗噪点，不然看不见
        painter.setPen(pen);
        painter.drawPoint(randomGenerator.generate() % width,randomGenerator.generate() % height);
    }

    // 随机干扰线
    for(int i = 0; i < 5; i++)
    {
        pen = QPen(QColor(randomGenerator.generate() % 256,randomGenerator.generate() % 256,randomGenerator.generate() % 256));
        pen.setWidth(1);
        painter.setPen(pen);
        painter.drawLine(randomGenerator.generate() % width,randomGenerator.generate() % height,randomGenerator.generate() % width,QRandomGenerator::global()->generate() % height);
    }

    // 绘制验证码文字（重点修复drawText）
    for(int i = 0; i < verifyCode.size(); i++)
    {
        pen = QPen(QColor(randomGenerator.generate() % 256,randomGenerator.generate() % 256,randomGenerator.generate() % 256));
        painter.setPen(pen);
        // x基准，y基准 + 小随机偏移，文字框30*30
        int x = 5 + 20 * i;
        int y = 15 + QRandomGenerator::global()->generate() % 10;
        painter.drawText(x, y, 30, 30, Qt::AlignCenter, verifyCode[i]);
    }
}

void VerifyCodeWidget::mousePressEvent(QMouseEvent *event)
{
    this->refrushVerifyCode();
}
