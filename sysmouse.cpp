#include "sysmouse.h"

#if plfrm == linos

SysMouse::SysMouse() {
    display = XOpenDisplay(NULL);
    button = Button1;
}

void SysMouse::mouseEvent(QDataStream &stream) {

    qint8 type;
    stream >> type;

    qDebug() << "SysRead";

    switch (type) {

        case ENMouse::Move: {
            QPoint point;
            stream >> point;

            //qDebug() << "received: " << point << "current: " << QCursor::pos();

            QCursor::setPos(point);

        } break;

        case ENMouse::LClick: mouseClick(); break;

    }
    qDebug() << "SysFinish";
}

void SysMouse::mouseClick()
{
    qDebug() << "SysClicked";
    static int press_release = 1;
    // Create and setting up the event

    XEvent event;

    memset (&event, 0, sizeof (event));
    event.xbutton.button = Button1;
    event.xbutton.same_screen = True;
    event.xbutton.subwindow = DefaultRootWindow (display);

    while (event.xbutton.subwindow) {

        event.xbutton.window = event.xbutton.subwindow;

        XQueryPointer (display, event.xbutton.window,
               &event.xbutton.root, &event.xbutton.subwindow,
               &event.xbutton.x_root, &event.xbutton.y_root,
               &event.xbutton.x, &event.xbutton.y,
               &event.xbutton.state);
    }

    // Press
    event.type = ButtonPress;

    if(press_release){

        if (XSendEvent (display, PointerWindow, True, ButtonPressMask | ButtonMotionMask, &event) == 0)
            fprintf (stderr, "Error to send the event!\n");

        //XFlush (display);
        usleep (10);

    }
    // Release

    if(!press_release){

        event.type = ButtonRelease;

        if (XSendEvent (display, PointerWindow, True, ButtonReleaseMask | ButtonMotionMask, &event) == 0)
            fprintf (stderr, "Error to send the event!\n");

        XFlush (display);
        usleep (10);
    }

    press_release = !press_release;

}

void SysMouse::mouseDoubleClick()
{

}

#endif
