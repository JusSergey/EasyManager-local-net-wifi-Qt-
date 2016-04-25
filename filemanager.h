#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QtWidgets>
#include <QtNetwork>
#include "constant.h"
#include "function.h"
#include "mytoolbutton.h"
#include "mylistwidget.h"
//#include "server.h"

class FileManager : public QWidget
{
    Q_OBJECT
public:
    explicit FileManager(quint16 port, QWidget *parent);
    ~FileManager();

private:    //visual
    QLineEdit*    lineIp;
    QLineEdit*    linePort;
    MyToolButton*  buttonConnect;
    MyToolButton*  buttonPathDir;
    MyToolButton*  buttonPause;
    MyToolButton*  buttonSelect;
    MyToolButton*  buttonSend;
    MyToolButton*  buttonClear;
    QString       stringStandartPath;
    QLabel*       labelStatus;
    QLabel        labelSpeed;

    QProgressBar* allFiles;

    QHBoxLayout* layoutPort;
    QHBoxLayout* layoutIp;
    QTimer       timerConnectToServer;

    QPalette *palette;
    QBrush *brush;

    MyListWidget *listWidget;
    MyListWidget *senderWidget;
    QMap<QString, QString> senders;

    QString netPath;

private:    //data
    QTcpSocket* tcpSocket;
    quint16     blockFile;
    quint32     indexFile;
    qint32      valueFiles;
    QFile       file;
    bool        statusSend;
    bool        flagSelect;

    QTimer *t;

private:
    inline QString getFormatStr(const QString& str, int maxSymbols);
    void setEnablePauseButton();

protected:
    void resizeEvent(QResizeEvent* event);

private slots:
    void slotConnectToServer(bool click = false);
    void slotReadyRead();
    void slotGetPackage();
    void slotPathDir();
    void slotPause();
    void slotButtonConnect();

    void slotOpenNextFile(QDataStream &stream);
    void slotFileData(QDataStream &stream);
    void slotCountFiles(QDataStream &stream);
    void slotDirName(QDataStream &stream);
    void slotListFileDir(QDataStream &stream);
    void slotConnect();
    void slotFinish();

    //void slotResize(int w, int h);

    //list widget
    void slotClickItem(QListWidgetItem* item);
    void slotButtonSelect();
    void slotButtonSend();

signals:
    void signalGetPackage();
    void fin();

};

#endif // FILEMANAGER_H
