#ifndef MUSICSLIDER_H
#define MUSICSLIDER_H

#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class MusicSlider;
}

class MusicSlider : public QWidget
{
    Q_OBJECT

public:
    explicit MusicSlider(QWidget *parent = nullptr);
    ~MusicSlider();

    void setStep(float ratio);//进度条移动

protected:
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

    void moveSlider();

signals:
    void setMusicSliderPosition(float ratio);
private:
    Ui::MusicSlider *ui;

    int currentPos;     // 播放进度
    int maxWidth;       // outLine总宽度
};

#endif // MUSICSLIDER_H
