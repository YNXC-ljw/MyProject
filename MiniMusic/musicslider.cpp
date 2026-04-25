#include "musicslider.h"
#include "ui_musicslider.h"

MusicSlider::MusicSlider(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MusicSlider)
{
    ui->setupUi(this);
    maxWidth = this->width();
//    setStyleSheet("background-color: yellow");
    setAutoFillBackground(true);
}

MusicSlider::~MusicSlider()
{
    delete ui;
}

void MusicSlider::setStep(float ratio)
{
    currentPos = maxWidth * ratio;
    moveSlider();
}

void MusicSlider::mousePressEvent(QMouseEvent *event)
{
    currentPos = event->pos().x();
    moveSlider();
}

void MusicSlider::mouseMoveEvent(QMouseEvent *event)
{
    // 鼠标移动的时候一定要在MusicSlider范围内
    QRect musicSliderRect = QRect(0,0,geometry().width(),geometry().height());
    if(!musicSliderRect.contains(event->pos()))
    {
        return;
    }


    if(event->buttons() == Qt::LeftButton)
    {

    }
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

void MusicSlider::mouseReleaseEvent(QMouseEvent *event)
{
    currentPos = event->pos().x();
    moveSlider();
    //移动时发不发信号都一样，但是鼠标释放时一定要发
    emit setMusicSliderPosition(ui->outLine->width()/(float)maxWidth);
}

void MusicSlider::moveSlider()
{
    ui->outLine->setGeometry(ui->outLine->x(),ui->outLine->y(),currentPos,ui->outLine->height());
}
