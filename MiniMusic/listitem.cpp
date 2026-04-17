#include "listitem.h"
#include "ui_listitem.h"

ListItem::ListItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListItem)
{
    ui->setupUi(this);
}

ListItem::~ListItem()
{
    delete ui;
}

void ListItem::enterEvent(QEvent *event)
{
    (void)event;
    setStyleSheet("background-color: #66ffff");
}

void ListItem::leaveEvent(QEvent *event)
{
    (void)event;
    setStyleSheet("");
}
