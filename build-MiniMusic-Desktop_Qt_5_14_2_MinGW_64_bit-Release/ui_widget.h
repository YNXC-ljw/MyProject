/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <musicslider.h>
#include "btform.h"
#include "commonpage.h"
#include "recbox.h"

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QWidget *background;
    QVBoxLayout *verticalLayout;
    QWidget *head;
    QHBoxLayout *horizontalLayout_2;
    QWidget *headleft;
    QHBoxLayout *horizontalLayout_4;
    QLabel *logo;
    QWidget *headright;
    QHBoxLayout *horizontalLayout_3;
    QWidget *searchBox;
    QHBoxLayout *horizontalLayout_5;
    QLineEdit *search;
    QWidget *settingBox;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer;
    QPushButton *skin;
    QPushButton *max;
    QPushButton *min;
    QPushButton *quit;
    QWidget *body;
    QHBoxLayout *horizontalLayout_7;
    QWidget *bodyleft;
    QVBoxLayout *verticalLayout_2;
    QWidget *leftBox;
    QVBoxLayout *verticalLayout_3;
    QWidget *onlineMusic;
    QVBoxLayout *verticalLayout_4;
    QLabel *onlineMusicText;
    BtForm *Rec;
    BtForm *audio;
    BtForm *music;
    QWidget *myMusic;
    QVBoxLayout *verticalLayout_5;
    QLabel *myMusicText;
    BtForm *like;
    BtForm *local;
    BtForm *recent;
    QSpacerItem *verticalSpacer;
    QWidget *bodyright;
    QVBoxLayout *verticalLayout_6;
    QStackedWidget *stackedWidget;
    QWidget *recPage;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_7;
    QLabel *recText;
    QLabel *recMusicText;
    RecBox *recMusicBox;
    QLabel *supplyMusicText;
    RecBox *supplyMusicBox;
    QWidget *radioPage;
    QLabel *label_2;
    QWidget *musicPage;
    QLabel *label_3;
    CommonPage *likePage;
    CommonPage *localPage;
    CommonPage *recentPage;
    MusicSlider *progressBar;
    QWidget *controlBox;
    QHBoxLayout *horizontalLayout_8;
    QWidget *play1;
    QGridLayout *gridLayout;
    QLabel *musicCover;
    QLabel *musicName;
    QLabel *musicSinger;
    QWidget *play2;
    QHBoxLayout *horizontalLayout_9;
    QPushButton *playModel;
    QPushButton *playUp;
    QPushButton *play;
    QPushButton *playDown;
    QPushButton *volume;
    QPushButton *addLocal;
    QWidget *play3;
    QHBoxLayout *horizontalLayout;
    QLabel *labelNULL;
    QLabel *currentTime;
    QLabel *line;
    QLabel *totalTime;
    QPushButton *lrcWord;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(1047, 697);
        background = new QWidget(Widget);
        background->setObjectName(QString::fromUtf8("background"));
        background->setGeometry(QRect(9, 9, 1029, 679));
        background->setStyleSheet(QString::fromUtf8(""));
        verticalLayout = new QVBoxLayout(background);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        head = new QWidget(background);
        head->setObjectName(QString::fromUtf8("head"));
        head->setMinimumSize(QSize(0, 80));
        head->setMaximumSize(QSize(16777215, 80));
        head->setStyleSheet(QString::fromUtf8("#head{\n"
"	background-color: #66ffcc;\n"
"}"));
        horizontalLayout_2 = new QHBoxLayout(head);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        headleft = new QWidget(head);
        headleft->setObjectName(QString::fromUtf8("headleft"));
        headleft->setMinimumSize(QSize(200, 0));
        headleft->setMaximumSize(QSize(200, 16777215));
        headleft->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout_4 = new QHBoxLayout(headleft);
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        logo = new QLabel(headleft);
        logo->setObjectName(QString::fromUtf8("logo"));
        logo->setStyleSheet(QString::fromUtf8("#logo{\n"
"	background-image:url(\":/image/logo.png\");\n"
"	background-repeat: no-repeat;\n"
"	background-position: center center;\n"
"}"));

        horizontalLayout_4->addWidget(logo);


        horizontalLayout_2->addWidget(headleft);

        headright = new QWidget(head);
        headright->setObjectName(QString::fromUtf8("headright"));
        headright->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout_3 = new QHBoxLayout(headright);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        searchBox = new QWidget(headright);
        searchBox->setObjectName(QString::fromUtf8("searchBox"));
        searchBox->setMinimumSize(QSize(300, 35));
        searchBox->setMaximumSize(QSize(300, 35));
        searchBox->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout_5 = new QHBoxLayout(searchBox);
        horizontalLayout_5->setSpacing(0);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(8, 0, 0, 0);
        search = new QLineEdit(searchBox);
        search->setObjectName(QString::fromUtf8("search"));
        search->setMinimumSize(QSize(0, 35));
        search->setStyleSheet(QString::fromUtf8("#search{\n"
"	background-color: #EAEAEA;\n"
"	border: none;\n"
"	border-radius: 17px;\n"
"	padding-left: 17px;\n"
"}"));

        horizontalLayout_5->addWidget(search);


        horizontalLayout_3->addWidget(searchBox);

        settingBox = new QWidget(headright);
        settingBox->setObjectName(QString::fromUtf8("settingBox"));
        settingBox->setStyleSheet(QString::fromUtf8("QPushButton:hover\n"
"{\n"
"	background-color:#FFFFFF;\n"
"}\n"
"\n"
"#quit:hover\n"
"{\n"
"	background-color: rgba(196, 43, 28,0.8);\n"
"}"));
        horizontalLayout_6 = new QHBoxLayout(settingBox);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalSpacer = new QSpacerItem(349, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer);

        skin = new QPushButton(settingBox);
        skin->setObjectName(QString::fromUtf8("skin"));
        skin->setMinimumSize(QSize(0, 0));
        skin->setMaximumSize(QSize(30, 30));
        skin->setStyleSheet(QString::fromUtf8("#skin {\n"
"    background-image: url(:/image/skin.png);\n"
"    border: none;\n"
"    background-repeat: no-repeat;\n"
"    background-position: center center;\n"
"    padding: 0px; \n"
"}"));

        horizontalLayout_6->addWidget(skin);

        max = new QPushButton(settingBox);
        max->setObjectName(QString::fromUtf8("max"));
        max->setMinimumSize(QSize(30, 30));
        max->setStyleSheet(QString::fromUtf8("#max{\n"
"	border: none;\n"
"	background-image: url(\":/image/max.png\");\n"
"}"));

        horizontalLayout_6->addWidget(max);

        min = new QPushButton(settingBox);
        min->setObjectName(QString::fromUtf8("min"));
        min->setMinimumSize(QSize(30, 30));
        min->setStyleSheet(QString::fromUtf8("#min{\n"
"	border: none;\n"
"}"));

        horizontalLayout_6->addWidget(min);

        quit = new QPushButton(settingBox);
        quit->setObjectName(QString::fromUtf8("quit"));
        quit->setMinimumSize(QSize(30, 30));
        quit->setStyleSheet(QString::fromUtf8("#quit{\n"
"	border: none;\n"
"}"));

        horizontalLayout_6->addWidget(quit);


        horizontalLayout_3->addWidget(settingBox);


        horizontalLayout_2->addWidget(headright);


        verticalLayout->addWidget(head);

        body = new QWidget(background);
        body->setObjectName(QString::fromUtf8("body"));
        body->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout_7 = new QHBoxLayout(body);
        horizontalLayout_7->setSpacing(0);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(0, 0, 0, 0);
        bodyleft = new QWidget(body);
        bodyleft->setObjectName(QString::fromUtf8("bodyleft"));
        bodyleft->setMinimumSize(QSize(200, 0));
        bodyleft->setMaximumSize(QSize(200, 16777215));
        bodyleft->setStyleSheet(QString::fromUtf8("#bodyleft{ background-color: #66ffcc; }"));
        verticalLayout_2 = new QVBoxLayout(bodyleft);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        leftBox = new QWidget(bodyleft);
        leftBox->setObjectName(QString::fromUtf8("leftBox"));
        leftBox->setMinimumSize(QSize(0, 400));
        leftBox->setStyleSheet(QString::fromUtf8(""));
        verticalLayout_3 = new QVBoxLayout(leftBox);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        onlineMusic = new QWidget(leftBox);
        onlineMusic->setObjectName(QString::fromUtf8("onlineMusic"));
        onlineMusic->setStyleSheet(QString::fromUtf8(""));
        verticalLayout_4 = new QVBoxLayout(onlineMusic);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        onlineMusicText = new QLabel(onlineMusic);
        onlineMusicText->setObjectName(QString::fromUtf8("onlineMusicText"));

        verticalLayout_4->addWidget(onlineMusicText);

        Rec = new BtForm(onlineMusic);
        Rec->setObjectName(QString::fromUtf8("Rec"));
        Rec->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_4->addWidget(Rec);

        audio = new BtForm(onlineMusic);
        audio->setObjectName(QString::fromUtf8("audio"));
        audio->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_4->addWidget(audio);

        music = new BtForm(onlineMusic);
        music->setObjectName(QString::fromUtf8("music"));
        music->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_4->addWidget(music);


        verticalLayout_3->addWidget(onlineMusic);

        myMusic = new QWidget(leftBox);
        myMusic->setObjectName(QString::fromUtf8("myMusic"));
        myMusic->setStyleSheet(QString::fromUtf8(""));
        verticalLayout_5 = new QVBoxLayout(myMusic);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        myMusicText = new QLabel(myMusic);
        myMusicText->setObjectName(QString::fromUtf8("myMusicText"));

        verticalLayout_5->addWidget(myMusicText);

        like = new BtForm(myMusic);
        like->setObjectName(QString::fromUtf8("like"));
        like->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_5->addWidget(like);

        local = new BtForm(myMusic);
        local->setObjectName(QString::fromUtf8("local"));
        local->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_5->addWidget(local);

        recent = new BtForm(myMusic);
        recent->setObjectName(QString::fromUtf8("recent"));
        recent->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_5->addWidget(recent);


        verticalLayout_3->addWidget(myMusic);


        verticalLayout_2->addWidget(leftBox);

        verticalSpacer = new QSpacerItem(20, 199, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        horizontalLayout_7->addWidget(bodyleft);

        bodyright = new QWidget(body);
        bodyright->setObjectName(QString::fromUtf8("bodyright"));
        bodyright->setStyleSheet(QString::fromUtf8(""));
        verticalLayout_6 = new QVBoxLayout(bodyright);
        verticalLayout_6->setSpacing(0);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        stackedWidget = new QStackedWidget(bodyright);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidget->setStyleSheet(QString::fromUtf8("background-color:#66FFCC;"));
        recPage = new QWidget();
        recPage->setObjectName(QString::fromUtf8("recPage"));
        scrollArea = new QScrollArea(recPage);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setGeometry(QRect(0, 0, 820, 500));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 818, 498));
        verticalLayout_7 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        recText = new QLabel(scrollAreaWidgetContents);
        recText->setObjectName(QString::fromUtf8("recText"));
        recText->setMinimumSize(QSize(0, 50));
        recText->setMaximumSize(QSize(16777215, 50));
        QFont font;
        font.setPointSize(24);
        recText->setFont(font);

        verticalLayout_7->addWidget(recText);

        recMusicText = new QLabel(scrollAreaWidgetContents);
        recMusicText->setObjectName(QString::fromUtf8("recMusicText"));
        recMusicText->setMinimumSize(QSize(0, 30));
        recMusicText->setMaximumSize(QSize(16777215, 30));
        QFont font1;
        font1.setPointSize(18);
        recMusicText->setFont(font1);

        verticalLayout_7->addWidget(recMusicText);

        recMusicBox = new RecBox(scrollAreaWidgetContents);
        recMusicBox->setObjectName(QString::fromUtf8("recMusicBox"));

        verticalLayout_7->addWidget(recMusicBox);

        supplyMusicText = new QLabel(scrollAreaWidgetContents);
        supplyMusicText->setObjectName(QString::fromUtf8("supplyMusicText"));
        supplyMusicText->setMinimumSize(QSize(0, 30));
        supplyMusicText->setMaximumSize(QSize(16777215, 30));
        supplyMusicText->setFont(font1);

        verticalLayout_7->addWidget(supplyMusicText);

        supplyMusicBox = new RecBox(scrollAreaWidgetContents);
        supplyMusicBox->setObjectName(QString::fromUtf8("supplyMusicBox"));

        verticalLayout_7->addWidget(supplyMusicBox);

        scrollArea->setWidget(scrollAreaWidgetContents);
        stackedWidget->addWidget(recPage);
        radioPage = new QWidget();
        radioPage->setObjectName(QString::fromUtf8("radioPage"));
        label_2 = new QLabel(radioPage);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(360, 240, 81, 31));
        stackedWidget->addWidget(radioPage);
        musicPage = new QWidget();
        musicPage->setObjectName(QString::fromUtf8("musicPage"));
        label_3 = new QLabel(musicPage);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(360, 240, 81, 31));
        stackedWidget->addWidget(musicPage);
        likePage = new CommonPage();
        likePage->setObjectName(QString::fromUtf8("likePage"));
        stackedWidget->addWidget(likePage);
        localPage = new CommonPage();
        localPage->setObjectName(QString::fromUtf8("localPage"));
        stackedWidget->addWidget(localPage);
        recentPage = new CommonPage();
        recentPage->setObjectName(QString::fromUtf8("recentPage"));
        stackedWidget->addWidget(recentPage);

        verticalLayout_6->addWidget(stackedWidget);

        progressBar = new MusicSlider(bodyright);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setMinimumSize(QSize(0, 25));
        progressBar->setMaximumSize(QSize(16777215, 25));
        progressBar->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_6->addWidget(progressBar);

        controlBox = new QWidget(bodyright);
        controlBox->setObjectName(QString::fromUtf8("controlBox"));
        controlBox->setMinimumSize(QSize(0, 70));
        controlBox->setMaximumSize(QSize(16777215, 70));
        controlBox->setStyleSheet(QString::fromUtf8("background-color:#CCFFFF;"));
        horizontalLayout_8 = new QHBoxLayout(controlBox);
        horizontalLayout_8->setSpacing(0);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(0, 0, 0, 2);
        play1 = new QWidget(controlBox);
        play1->setObjectName(QString::fromUtf8("play1"));
        play1->setStyleSheet(QString::fromUtf8(""));
        gridLayout = new QGridLayout(play1);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 2);
        musicCover = new QLabel(play1);
        musicCover->setObjectName(QString::fromUtf8("musicCover"));
        musicCover->setMinimumSize(QSize(60, 60));
        musicCover->setMaximumSize(QSize(60, 60));

        gridLayout->addWidget(musicCover, 0, 0, 2, 1);

        musicName = new QLabel(play1);
        musicName->setObjectName(QString::fromUtf8("musicName"));

        gridLayout->addWidget(musicName, 0, 1, 1, 1);

        musicSinger = new QLabel(play1);
        musicSinger->setObjectName(QString::fromUtf8("musicSinger"));

        gridLayout->addWidget(musicSinger, 1, 1, 1, 1);


        horizontalLayout_8->addWidget(play1);

        play2 = new QWidget(controlBox);
        play2->setObjectName(QString::fromUtf8("play2"));
        play2->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	border: none;\n"
"	background-repeat: no-repeat;\n"
"	background-position: center center;\n"
"}\n"
"QPushButton:hover\n"
"{\n"
"	background-color: #33FF99;\n"
"}"));
        horizontalLayout_9 = new QHBoxLayout(play2);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        playModel = new QPushButton(play2);
        playModel->setObjectName(QString::fromUtf8("playModel"));
        playModel->setMinimumSize(QSize(30, 30));
        playModel->setMaximumSize(QSize(30, 30));

        horizontalLayout_9->addWidget(playModel);

        playUp = new QPushButton(play2);
        playUp->setObjectName(QString::fromUtf8("playUp"));
        playUp->setMinimumSize(QSize(30, 30));
        playUp->setMaximumSize(QSize(30, 30));

        horizontalLayout_9->addWidget(playUp);

        play = new QPushButton(play2);
        play->setObjectName(QString::fromUtf8("play"));
        play->setMinimumSize(QSize(30, 30));
        play->setMaximumSize(QSize(30, 30));

        horizontalLayout_9->addWidget(play);

        playDown = new QPushButton(play2);
        playDown->setObjectName(QString::fromUtf8("playDown"));
        playDown->setMinimumSize(QSize(30, 30));
        playDown->setMaximumSize(QSize(30, 30));

        horizontalLayout_9->addWidget(playDown);

        volume = new QPushButton(play2);
        volume->setObjectName(QString::fromUtf8("volume"));
        volume->setMinimumSize(QSize(30, 30));
        volume->setMaximumSize(QSize(30, 30));

        horizontalLayout_9->addWidget(volume);

        addLocal = new QPushButton(play2);
        addLocal->setObjectName(QString::fromUtf8("addLocal"));
        addLocal->setMinimumSize(QSize(30, 30));
        addLocal->setMaximumSize(QSize(30, 30));

        horizontalLayout_9->addWidget(addLocal);


        horizontalLayout_8->addWidget(play2);

        play3 = new QWidget(controlBox);
        play3->setObjectName(QString::fromUtf8("play3"));
        play3->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout = new QHBoxLayout(play3);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        labelNULL = new QLabel(play3);
        labelNULL->setObjectName(QString::fromUtf8("labelNULL"));
        labelNULL->setMinimumSize(QSize(120, 0));
        labelNULL->setMaximumSize(QSize(120, 16777215));

        horizontalLayout->addWidget(labelNULL);

        currentTime = new QLabel(play3);
        currentTime->setObjectName(QString::fromUtf8("currentTime"));

        horizontalLayout->addWidget(currentTime);

        line = new QLabel(play3);
        line->setObjectName(QString::fromUtf8("line"));

        horizontalLayout->addWidget(line);

        totalTime = new QLabel(play3);
        totalTime->setObjectName(QString::fromUtf8("totalTime"));

        horizontalLayout->addWidget(totalTime);

        lrcWord = new QPushButton(play3);
        lrcWord->setObjectName(QString::fromUtf8("lrcWord"));
        lrcWord->setMinimumSize(QSize(30, 30));
        lrcWord->setMaximumSize(QSize(30, 30));

        horizontalLayout->addWidget(lrcWord);


        horizontalLayout_8->addWidget(play3);


        verticalLayout_6->addWidget(controlBox);


        horizontalLayout_7->addWidget(bodyright);


        verticalLayout->addWidget(body);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        logo->setText(QString());
        search->setPlaceholderText(QString());
        skin->setText(QString());
        max->setText(QString());
        min->setText(QString());
        quit->setText(QString());
        onlineMusicText->setText(QCoreApplication::translate("Widget", "\345\234\250\347\272\277\351\237\263\344\271\220", nullptr));
        myMusicText->setText(QCoreApplication::translate("Widget", "\346\210\221\347\232\204\351\237\263\344\271\220", nullptr));
        recText->setText(QCoreApplication::translate("Widget", "\346\216\250\350\215\220", nullptr));
        recMusicText->setText(QCoreApplication::translate("Widget", "\344\273\212\346\227\245\344\270\272\344\275\240\346\216\250\350\215\220", nullptr));
        supplyMusicText->setText(QCoreApplication::translate("Widget", "\344\275\240\347\232\204\351\237\263\344\271\220\350\241\245\347\273\231\347\253\231", nullptr));
        label_2->setText(QCoreApplication::translate("Widget", "\346\225\254\350\257\267\346\234\237\345\276\205", nullptr));
        label_3->setText(QCoreApplication::translate("Widget", "\346\225\254\350\257\267\346\234\237\345\276\205", nullptr));
        musicCover->setText(QCoreApplication::translate("Widget", "\345\233\276\347\211\207", nullptr));
        musicName->setText(QCoreApplication::translate("Widget", "\346\255\214\345\220\215", nullptr));
        musicSinger->setText(QCoreApplication::translate("Widget", "\346\255\214\346\211\213", nullptr));
        playModel->setText(QString());
        playUp->setText(QString());
        play->setText(QString());
        playDown->setText(QString());
        volume->setText(QString());
        addLocal->setText(QString());
        labelNULL->setText(QString());
        currentTime->setText(QCoreApplication::translate("Widget", "00:00", nullptr));
        line->setText(QCoreApplication::translate("Widget", "/", nullptr));
        totalTime->setText(QCoreApplication::translate("Widget", "00:00", nullptr));
        lrcWord->setText(QCoreApplication::translate("Widget", "\350\257\215", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
