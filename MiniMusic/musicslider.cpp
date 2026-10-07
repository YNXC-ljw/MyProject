#include "musicslider.h"
#include "ui_musicslider.h"

MusicSlider::MusicSlider(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MusicSlider)
{
    ui->setupUi(this);
    maxWidth = this->width();

    setAutoFillBackground(true);
}

MusicSlider::~MusicSlider()
{
    delete ui;
}

// 移动进度条
void MusicSlider::moveSlider()
{
    ui->outLine->setGeometry(ui->outLine->x(),ui->outLine->y(),currentPos,ui->outLine->height());
    ui->outLine->setStyleSheet("#outLine{ background-color:#1ECC94; }");
}

// 播放进度在不断变化，需要一直通过播放进度更新播放进度条
void MusicSlider::setStep(float ratio)
{
    currentPos = maxWidth * ratio;
    moveSlider();
}

// 鼠标点击事件
void MusicSlider::mousePressEvent(QMouseEvent *event)
{
    currentPos = event->pos().x();
    moveSlider();
}

// 鼠标移动事件
void MusicSlider::mouseMoveEvent(QMouseEvent *event)
{
    // 鼠标移动的时候一定要在MusicSlider范围内
    QRect musicSliderRect = QRect(0,0,geometry().width(),geometry().height());
    // 鼠标移动到控件外直接返回
    if(!musicSliderRect.contains(event->pos()))
    {
        return;
    }

    if(event->buttons() == Qt::LeftButton)
    {
        currentPos = event->pos().x();
        if(currentPos < 0)
        {
            currentPos = 0;
        }
        if(currentPos > maxWidth)
        {
            currentPos = maxWidth;
        }
        moveSlider();
    }
}

// 鼠标释放事件
void MusicSlider::mouseReleaseEvent(QMouseEvent *event)
{
    currentPos = event->pos().x();
    moveSlider();
    //移动时发不发信号都一样，但是鼠标释放时一定要发
    emit setMusicSliderPosition(ui->outLine->width()/(float)maxWidth);
}

