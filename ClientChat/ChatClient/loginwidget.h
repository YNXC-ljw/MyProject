#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include "verifycodewidget.h"
#include <QPushButton>

class LoginWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LoginWidget(QWidget *parent = nullptr);

    // 切换登录/注册界面
    void switchMode();

    void onClickSubmitBtn();
    void userLoginDone(bool ok,const QString& reason);
    void userRegisterDone(bool ok,const QString& reason);
private:
    bool isLoginMode = true;

    QLineEdit* usernameEdit;
    QLineEdit* passwordEdit;
    QLineEdit* verifyCodeEdit;

    QLabel* titleLabel;
    QPushButton* submitBtn;
    QPushButton* phoneModeBtn;
    QPushButton* switchModeBtn;

    VerifyCodeWidget* verifyCodeWidget;
signals:
};

#endif // LOGINWIDGET_H
