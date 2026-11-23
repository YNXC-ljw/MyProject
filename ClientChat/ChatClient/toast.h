#ifndef TOAST_H
#define TOAST_H

#include <QDialog>
#include <QWidget>

class Toast : public QDialog
{
    Q_OBJECT
public:
    Toast(const QString& text);
    // 并不需要手动new这个对象，而是通过showMessage来弹出窗口
    static void showMessage(const QString& text);
};

#endif // TOAST_H
