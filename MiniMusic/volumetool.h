#ifndef VOLUMETOOL_H
#define VOLUMETOOL_H

#include <QWidget>
#include <QPaintEvent>

namespace Ui {
class VolumeTool;
}

class VolumeTool : public QWidget
{
    Q_OBJECT

public:
    explicit VolumeTool(QWidget *parent = nullptr);
    ~VolumeTool();

    void paintEvent(QPaintEvent *event);

    void onSilenceBtnClicked();
    void eventfilter(QObject* watched,QEvent* event);
signals:
    void setMusicMuted(bool);

private:
    Ui::VolumeTool *ui;
    bool isMuted;
    int volumeRatio;//标记音量大小
};

#endif // VOLUMETOOL_H
