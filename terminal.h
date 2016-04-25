#ifndef TERMINAL_H
#define TERMINAL_H

#include <QtWidgets>
#include <QtNetwork>
#include "function.h"
#include "constant.h"
#include "mytoolbutton.h"

class Terminal : public QWidget
{
    Q_OBJECT
public:
    explicit Terminal(QWidget *parent = 0);
    ~Terminal();

private:
    QTcpSocket* socket;

private:
    QLabel *labelPort;
    QLabel *labelIP;

    QLineEdit *linePort;
    QLineEdit *lineIp;
    MyToolButton *buttonConnect;

    QTextEdit *textInfo;
    QLineEdit *lineCommand;
    MyToolButton *buttonCommand;

private:
    QVBoxLayout *layout;
    QHBoxLayout *HLayout;

    QHBoxLayout *layoutPort;
    QHBoxLayout *layoutIP;

    quint16 blockFile;

private slots:
    void slotConnect();
    void slotCommand();
    void slotReadyRead();

};

#endif // TERMINAL_H
