#include "commonpage.h"
#include "ui_commonpage.h"
#include "listitem.h"

CommonPage::CommonPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommonPage)
{
    ui->setupUi(this);
}

CommonPage::~CommonPage()
{
    delete ui;
}

void CommonPage::setCommonPageUi(const QString& text,const QString& imagePath)
{
    ui->pageTitle->setText(text);
    ui->musicImageLabel->setPixmap(QPixmap(imagePath));
    ui->musicImageLabel->setScaledContents(true);

    //测试代码：
    ListItem* listItem = new ListItem(this);
    QListWidgetItem* item = new QListWidgetItem(ui->pageMusicList);
    item->setSizeHint(QSize(listItem->width(),listItem->height()));
    ui->pageMusicList->setItemWidget(item,listItem);
}
