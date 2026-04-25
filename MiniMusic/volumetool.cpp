#include "volumetool.h"
#include "ui_volumetool.h"

#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QDebug>

VolumeTool::VolumeTool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VolumeTool)
  ,isMuted(false)
  ,volumeRatio(20)
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

    connect(ui->silenceBtn,&QPushButton::clicked,this,&VolumeTool::onSilenceBtnClicked);
    //安装事件拦截器
    ui->volumeBox->installEventFilter(this);
}

VolumeTool::~VolumeTool()
{
    delete ui;
}
//绘制volumeTool的下三角
void VolumeTool::paintEvent(QPaintEvent *event)
{
    (void)event;

    QPainter painter(this);

    // 1. 设置画笔
    painter.setPen(Qt::NoPen);

    // 2. 设置画刷
    painter.setBrush(QBrush(Qt::white));

    // 3. 绘制三角
    QPolygon polygon;
    QPoint a(10+10,300);
    QPoint b(10+10+60,300);
    QPoint c(10+10+30,300+20);

    polygon.append(a);
    polygon.append(b);
    polygon.append(c);
    painter.drawPolygon(polygon);

}

void VolumeTool::onSilenceBtnClicked()
{
    isMuted = !isMuted;
    //给按钮设置图标
    if(isMuted)
    {
        ui->silenceBtn->setIcon(QIcon(":/image/silence.png"));
    }
    else
    {
        ui->silenceBtn->setIcon(QIcon(":/image/volume.png"));
    }

    //给Widget发送信号设置静音
    emit setMusicMuted(isMuted);
}

bool VolumeTool::eventFilter(QObject *watched, QEvent *event)
{
    if(ui->volumeBox == watched)//事件发生在volumeBox控件上
    {
        //鼠标按下
        if(event->type() == QEvent::MouseButtonPress)
        {
            calcVolume();
        }
        //鼠标释放
        else if(event->type() == QEvent::MouseButtonRelease)
        {
            emit setMusicVolume(volumeRatio);
        }
        //鼠标移动
        else if(event->type() == QEvent::MouseMove)
        {
            calcVolume();
            emit setMusicVolume(volumeRatio);
        }
        return true;
    }
    return QObject::eventFilter(watched,event);
}

void VolumeTool::calcVolume()
{
    //获取鼠标点击时的y坐标
    int height = ui->volumeBox->mapFromGlobal(QCursor().pos()).y();

    //验证height的合法性(超过高度就按最高计算，低于最低就按最低计算)
    height = height < 25 ? 25 : height;
    height = height > 205 ? 205 :height;

    //更新outline
    ui->outLine->setGeometry(ui->outLine->x(),height,ui->outLine->width(),205 - height);

    //更新sliderBtn位置
    ui->sliderBtn->move(ui->sliderBtn->x(),ui->outLine->y() - ui->sliderBtn->height()/2);

    //计算音量大小
    volumeRatio = (int)(ui->outLine->height()/(float)180*100);
    //更新QLabel
    ui->volumeRatio->setText(QString::number(volumeRatio)+"%");

}
