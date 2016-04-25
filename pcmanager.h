#ifndef PCMANAGER_H
#define PCMANAGER_H

#include <QtMath>
#include <QtWidgets>
#include <QtNetwork>
#include "function.h"
#include "constant.h"
#include "mytoolbutton.h"
#include "screendisplay.h"

#if plfrm == linos
#include "sysmouse.h"
#include <unistd.h>
#endif

class PCManager : public QWidget
{
    Q_OBJECT
public:
    explicit PCManager(QWidget *parent = 0);

private: //gui
    QLabel          labelIp;
    QLabel          labelPort;
    QLineEdit      *lineIp;
    QLineEdit      *linePort;

    QHBoxLayout    *lip;
    QHBoxLayout    *lport;

    QVBoxLayout    *layout;
    QHBoxLayout    *layoutScale;

    MyToolButton   *buttonConnect;

    QGraphicsView *view;

private:
    QTcpSocket           *socket;
    QGraphicsScene       *scene;
    QTimer               *timerUpdateDisplay;
    ScreenDisplay        *SDisplay;
    QSlider              *sliderScale;
    QLabel               *labelScale;

#if plfrm == linos
public:
    SysMouse mouse;
#endif

private:
    QDataStream *stream;
    QByteArray   array;

    int  timeMsec;
    bool isPress;
    bool is;

private:
    QPoint pressPoint;
    QPoint movedPoint;
    QTime  time;

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *key);

public slots:
    void slotSendDisplay(QTcpSocket *client);
    void slotConnect();
    void slotGetScreen();
    void slotSliderChande(int value);
    void slotReadyRead();

public:
    void getImages(QDataStream &stream);

public:
    inline int toMSec(const QTime &time1, const QTime &time2){
        return (abs((time2.second()*1000 + time2.msec()) - (time1.second()*1000 + time1.msec())));
    }

    inline bool len(const QPoint &p1, const QPoint &p2){
        return ((abs(p1.x()-p2.x())>16) || (abs(p1.y()-p2.y())>16) ? false : true);
    }


};

#endif // PCMANAGER_H
