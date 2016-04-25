#ifndef SERVER_H
#define SERVER_H

#include <QtWidgets>
#include <QtNetwork>
#include "constant.h"
#include "function.h"
#include "mytoolbutton.h"
#include "pcmanager.h"
#include "filemanager.h"
#include "terminal.h"

class Server : public QWidget
{
    Q_OBJECT
public:
    explicit Server(quint16 port_, PCManager *manager, QWidget *parent);
    ~Server();

public:    //visual
    MyToolButton*   buttonSend;
    MyToolButton*   buttonConnect;
    QTextEdit*      textInformation;
    QLineEdit*      linePort;
    QLabel*         labelStatus;

    QHBoxLayout     *horisontal;
    QVBoxLayout     *vertical;
    QVBoxLayout     *layout;

private:    //data
    QTcpServer*     tcpServer;
    QStringList     stringFilesSend;
    QStringList     stringAbsoluteFilesSend;
    QStringList     stringDirections;
    QTcpSocket*     tcpClient;
    QFile*          fileSend;
    qint32          nextBlockSize;
    quint16         blockFile;
    quint16         port;

    QMap<qint8, QTcpSocket*> Sockets;

    //QTcpSocket  *sterm;

    //SysMouse    *sysMouse;

    bool            isCreatedDirs;
    bool            isStartingServer;
    int             indexDir;
    int             indexFile;


private: //prt's tabs
    PCManager   *manager;

private slots:
    void slotSendFiles();
    void slotNewConnection();
    void slotDisconnected();
    void slotReadyRead();
    void slotOpenNextFile();
    void slotSendFile();
    void slotTextChange();
    void slotConnectServer();

private: // switch
    void connectManager();
    void terminal();
    void terminalCommand(QDataStream &stream);
    void fileManager();
    void listFileDir(QDataStream &stream);
    void getFiles(QDataStream &stream);
    //mouse
    void mousePress(QDataStream &stream);

signals:
    void signalOpenNextFile();
    void siganlWriteNextBlock();

private:
    inline QString getFormatStr(const QString& str, int maxSymbols);
    void Disconnected();
    void createListFiles();
    void createListDirAndFiles(QString path, QString parent);
    void setCorrectTextView();
    void sendNextDir();
    void setDefaultSetting();

};

#endif // SERVER_H
