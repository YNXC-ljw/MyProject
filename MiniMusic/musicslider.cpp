#include "musicslider.h"
#include "ui_musicslider.h"

MusicSlider::MusicSlider(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MusicSlider)
{
    ui->setupUi(this);
//    setStyleSheet("background-color: yellow");
    setAutoFillBackground(true);
}

MusicSlider::~MusicSlider()
{
    delete ui;
}
