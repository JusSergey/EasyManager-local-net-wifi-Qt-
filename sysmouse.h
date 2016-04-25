#ifndef SYSMOUSE_H
#define SYSMOUSE_H

#include <QtWidgets>
#include "constant.h"

#if (plfrm == linos)

#include <stdio.h>
#include <X11/Xlib.h>
#include <unistd.h>

class SysMouse
{

public:
    SysMouse();

private:
    Display *display;

private:
    int button;

public:
    void mouseEvent(QDataStream &stream);

public:
    void mouseClick();
    void mouseDoubleClick();

};
#endif

#endif // SYSMOUSE_H
