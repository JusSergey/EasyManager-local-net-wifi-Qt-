#include "mytoolbutton.h"

MyToolButton::MyToolButton(const QString &text,
                           const QString &backgroundColor,
                           const QString &textColor,
                           QWidget *parent) : QToolButton(parent)
{

    this->setText(text);
    setColor(backgroundColor, textColor);

    connect(this, SIGNAL(pressed()), this, SLOT(slotPress()));
    connect(this, SIGNAL(released()), this, SLOT(slotRelease()));

    this->setFixedHeight(buttonH);

}

void MyToolButton::setColor(const QString &backgroundColor, const QString &textColor)
{
    this->setStyleSheet("background: "+backgroundColor+"; color: "+textColor);
    Background = backgroundColor;
    Color = textColor;
}

void MyToolButton::slotPress()
{
    this->setStyleSheet("background: #AAAAAA; color: 111111");
}

void MyToolButton::slotRelease()
{
    setStyleSheet("background: "+Background+"; color: "+Color);
}

