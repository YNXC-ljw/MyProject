#include "listitem.h"
#include "ui_listitem.h"
#include <QDebug>

ListItem::ListItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListItem)
    ,isLike(false)
{
    ui->setupUi(this);

    connect(ui->likeBtn,&QPushButton::clicked,this,&ListItem::onLikeBtnClicked);
}

ListItem::~ListItem()
{
    //qDebug() << "ListItemBox被销毁";
    delete ui;
}

void ListItem::enterEvent(QEvent *event)
{
    (void)event;
    setStyleSheet("background-color: #66FFFF");
}

void ListItem::leaveEvent(QEvent *event)
{
    (void)event;
    setStyleSheet("");
}
void ListItem::setMusicName(const QString &musicName)
{
    ui->musicNameLabel->setText(musicName);
}

void ListItem::setMusicSinger(const QString &musicSinger)
{
    ui->musicSingerLabel->setText(musicSinger);
}

void ListItem::setMusicAlbum(const QString &musicAlbum)
{
    ui->musicAlbumLabel->setText(musicAlbum);
}

void ListItem::setLikeMusic(bool isLike)
{
    if(isLike)
    {
        ui->likeBtn->setIcon(QIcon(":/image/love.png"));
    }
    else
    {
        ui->likeBtn->setIcon(QIcon(":/image/unlove.png"));
    }
}

void ListItem::onLikeBtnClicked()
{
    isLike = !isLike;
    setLikeMusic(isLike);

    //收藏状态已经改变了,此时应该通知父元素即ListWidget(commonPage)来处理此music对象中的isLike属性
    emit setIsLike(isLike);
}
