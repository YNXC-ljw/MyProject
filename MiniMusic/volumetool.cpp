#include "volumetool.h"
#include "ui_volumetool.h"

#include<QGraphicsDropShadowEffect>

VolumeTool::VolumeTool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VolumeTool)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);//去掉背景

    // 窗口增加自定义的阴影效果
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setColor("#646464");
    shadowEffect->setBlurRadius(10);
    shadowEffect->setOffset(0, 0);
    setGraphicsEffect(shadowEffect);

    //设置图标
    ui->silenceBtn->setIcon(QIcon(":/image/volume.png"));

    ui->volumeRatio->setText("20%");
    // 设置 out_line 尺寸
    QRect rect = ui->outLine->geometry();
    // 窗体原本高度180*20%，25rect具体父元素上方的距离
    ui->outLine->setGeometry(rect.x(), 180 - 36 + 25, rect.width(), 36);

    //移动按钮位置
    ui->sliderBtn->move(ui->sliderBtn->x(),ui->outLine->y() - ui->sliderBtn->height()/2);
}

VolumeTool::~VolumeTool()
{
    delete ui;
}
