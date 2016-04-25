#ifndef MYTOOLBUTTON_H
#define MYTOOLBUTTON_H
#include <QtWidgets>
#include "constant.h"

class MyToolButton : public QToolButton
{

    Q_OBJECT

public:
    MyToolButton(const QString &text = "",
                 const QString &backgroundColor = "",
                 const QString &textColor = "",
                 QWidget *parent = nullptr);

    void setColor(const QString &backgroundColor = "", const QString &textColor = "");

private:
    QString Background;
    QString Color;

private slots:
    void slotPress();
    void slotRelease();


};

#endif // MYTOOLBUTTON_H
