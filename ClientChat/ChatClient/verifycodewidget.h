#ifndef VERIFYCODEWIDGET_H
#define VERIFYCODEWIDGET_H

#include <QWidget>
#include <QRandomGenerator>
class VerifyCodeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VerifyCodeWidget(QWidget *parent = nullptr);

    // 生成随机验证码
    QString generatorVerifyCode();
    // 检验验证码正确性
    bool cheakVerifyCode(const QString& verifyCode);
    // 重新生成验证码并将其渲染到界面上
    void refrushVerifyCode();

    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
private:
    // 随机数生成器
    QRandomGenerator randomGenerator;

    // 保存验证码
    QString verifyCode = "";
signals:
};

#endif // VERIFYCODEWIDGET_H
