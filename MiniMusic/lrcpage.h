#ifndef LRCPAGE_H
#define LRCPAGE_H

#include <QWidget>
#include <QPropertyAnimation>


struct LrcWordLine
{
    LrcWordLine(qint64 lrcTime,QString lrcText)
    {
        this->lrcTime = lrcTime;
        this->lrcText = lrcText;
    }
    qint64 lrcTime;
    QString lrcText;
};

namespace Ui {
class LrcPage;
}

class LrcPage : public QWidget
{
    Q_OBJECT

public:
    explicit LrcPage(QWidget *parent = nullptr);
    ~LrcPage();

    bool parseLrcFile(const QString& lrcPath);

    void showLrcWordLine(qint64 time);
    int getLrcWordLineIndex(qint64 time);
    QString getLrcWordByIndex(int index);

private:
    Ui::LrcPage *ui;

    QPropertyAnimation* animation;

    QVector<LrcWordLine> lrcWordLines;
};

#endif // LRCPAGE_H
