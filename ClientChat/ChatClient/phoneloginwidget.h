#ifndef PHONELOGINWIDGET_H
#define PHONELOGINWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QTimer>
#include <QPushButton>

class PhoneLoginWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PhoneLoginWidget(QWidget *parent = nullptr);

    void switchMode();

    void onClickSendVerifyCodeBtn();
    void getVerifyCodeDone();

    void onClickSubmitBtn();
    void phoneLoginDone(bool ok,const QString& reason);
    void phoneRegisterDone(bool ok,const QString& reason);
    // 倒计时
    void countDown();
private:
    bool isLoginMode = true;
    QLineEdit* phoneEdit;
    QPushButton* sendVerifyCodeBtn;
    QLineEdit* verifyCodeEdit;
    QLabel* titleLabel;
    QPushButton* submitBtn;
    QPushButton* switchModeBtn;

    QString currentPhone = "";

    // 定时器
    QTimer* timer;
    int time = 60;
signals:
};

#endif // PHONELOGINWIDGET_H
