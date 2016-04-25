#ifndef WIDGET_H
#define WIDGET_H

#include <QtWidgets>
#include "filemanager.h"
#include "server.h"
#include "pcmanager.h"
#include "function.h"
#include "constant.h"
#include "terminal.h"
#include "info.h"

class NetworkManager : public QTabWidget
{
    Q_OBJECT

public:
    NetworkManager(QWidget *parent = 0);
    ~NetworkManager();

private:
    QTabWidget  *widgets;
    FileManager *fileManager;
    Server      *server;
    Terminal    *terminal;
    PCManager   *manager;
    Info        *info;

private:
    QVBoxLayout *layoutTabs;

private slots:
    void slotShowFullScreen() { showFullScreen(); }
    void slotShowNormal()     { showNormal();     }
    void slotSetBackground(QString path);

};

#endif // WIDGET_H
