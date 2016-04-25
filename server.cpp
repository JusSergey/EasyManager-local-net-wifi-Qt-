#include "server.h"

Server::Server(quint16 port_, PCManager *manager, QWidget *parent)
    : QWidget(parent), nextBlockSize(0), blockFile(0), isStartingServer(false)
{

    {
        this->manager = manager;
    }

    port      = port_;

    linePort  = new QLineEdit(QString::number(port), this);
    tcpClient = nullptr;

    tcpServer = new QTcpServer(this);

    fileSend  = new QFile(this);

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));

    buttonSend       = new MyToolButton("&Надіслати", "#190227", "#C492F2", this);
    buttonConnect    = new MyToolButton("&Запустити", "#190227", "#C492F2", this);
    textInformation  = new QTextEdit(this);
    labelStatus      = new QLabel(this);

    horisontal = new QHBoxLayout;
    vertical   = new QVBoxLayout;
    layout     = new QVBoxLayout;

    horisontal->addWidget(new QLabel("Порт", this));
    horisontal->addWidget(linePort);
    horisontal->addWidget(buttonConnect);
    horisontal->addWidget(buttonSend);

    vertical->addWidget(textInformation);
    vertical->addWidget(labelStatus);

    layout->addLayout(horisontal);
    layout->addLayout(vertical);

    setLayout(layout);

    connect(buttonSend,       SIGNAL(clicked(bool)),         this, SLOT(slotSendFiles()));
    connect(buttonConnect,    SIGNAL(clicked(bool)),         this, SLOT(slotConnectServer()));
    connect(this,             SIGNAL(signalOpenNextFile()),  this, SLOT(slotOpenNextFile()));
    connect(textInformation,  SIGNAL(textChanged()),         this, SLOT(slotTextChange()));

    this->setGeometry(400, 400, 400, 300);

    //this->setMaximumWidth(500);

    buttonSend->setEnabled(false);

    setWindowTitle("Сервер");

    textInformation->setStyleSheet("background-color: #150121; color: #C492F2");
    //->setStyleSheet("background-color: #190227; color: #C492F2");
    linePort->setStyleSheet("background-color: #190227; color: #C492F2");
    //lineIp->setStyleSheet("background-color: #190227; color: #C492F2");

    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect;
    effect->setOpacity(0.8);

    setOpasityWidgets(effect, {linePort, textInformation});

    //sysMouse = new SysMouse();

//    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this);
//        shadowEffect -> setBlurRadius(26.0);
//        shadowEffect -> setColor(QColor(0, 0, 0, 140));
//        shadowEffect -> setOffset(9.0);

//        this->setGraphicsEffect(shadowEffect);

}

Server::~Server()
{
    Disconnected();
    delete horisontal;
    delete vertical;
    delete layout;
//    delete sysMouse;
}

void Server::slotConnectServer(){

    QString tmpPort = linePort->text();

    for(int i = 0; i < tmpPort.length(); i++)
        if(tmpPort[i] < '0' || tmpPort[i] > '9')
            return;

    port = tmpPort.toInt();

    if (buttonConnect->text() == "&Запустити") {

        linePort->setEnabled(false);

        if(!tcpServer->listen(QHostAddress::Any, port)){
            QMessageBox::critical(0, "Помилка", "Неможливо читати з порту: " + linePort->text());
            tcpServer->close();
            return;
        }
        if(isStartingServer)
            buttonSend->setEnabled(true);

        buttonConnect->setText("&Зупинити");
    }
    else {
        buttonConnect->setText("&Запустити");
        buttonSend->setEnabled(false);
        linePort->setEnabled(true);
        Disconnected();
    }

}

void Server::connectManager(){
    Sockets.insert(Command::MANAGER, (QTcpSocket*)(sender()));
}

void Server::terminal()
{
    Sockets.insert(Command::TERMINAL, (QTcpSocket*)(sender()));
}

void Server::fileManager()
{
    Sockets.insert(Command::FILE_MANAGER, (QTcpSocket*)(sender()));
}

void Server::terminalCommand(QDataStream &stream)
{

    QString command;
    stream >> command;
    QFile file("log_file.log");
    file.remove();

    system((command+" >> log_file.log").toStdString().c_str());

    if(!file.open(QIODevice::ReadOnly)){

        QMessageBox::information(nullptr, "(0-0)", "Can't create terminal file");
        return;

    }

    QByteArray array = file.readAll();
    file.close();
    file.remove();

    QByteArray tmp;
    QDataStream s(&tmp, QIODevice::WriteOnly);

    QString hostName = QHostInfo::localHostName();

    s << (quint16)(hostName.length() + array.size()+1)
      << qint8(Command::TERMINAL_COMMAND)
      << hostName
      << array;

    Sockets[Command::TERMINAL]->write(tmp);
}



void Server::listFileDir(QDataStream &stream)
{

    QString path;
    stream >> path;

    if(path == c_listWidgetGet+"/")
        path = "/";

    QFileInfoList list = QFileInfo(path+"/").dir().entryInfoList();


    QFile file_tmp("list_dir.log");
    file_tmp.remove();

    if(!file_tmp.open(QIODevice::WriteOnly)){
        QMessageBox::information(nullptr, "(-_-)", "list_dir.log");
        return;
    }

    file_tmp.write((path+"\n").toStdString().c_str());

    qDebug() << "sh: " << (path+"\n").toStdString().c_str();

    foreach(QFileInfo info, list){
        qDebug() << "getf: " << (info.fileName()+"\n");
        QString temp = QFileInfo(path+info.fileName()).isFile() ? "F" : "D";
        file_tmp.write((info.fileName()+temp+"\n").toStdString().c_str());
    }

    file_tmp.flush();
    file_tmp.close();

    if(!file_tmp.open(QIODevice::ReadOnly)){
        QMessageBox::information(nullptr, "(-_-)", "list_dir.log");
        return;
    }



    QByteArray dt = file_tmp.readAll();

    file_tmp.close();
    file_tmp.remove();

    QByteArray arr;
    QDataStream st(&arr, QIODevice::WriteOnly);

    st << quint16(1 + dt.size())
       << qint8(Command::LIST_FILE_DIR)
       << dt;

    Sockets[Command::FILE_MANAGER]->write(arr);


}

void Server::getFiles(QDataStream &stream)
{
    textInformation->clear();

    quint16 countFiles;
    stream >> countFiles;

    QString netPath;
    stream >> netPath;

    QString tempFilePath;



    for(quint16 i = 0; i < countFiles; i++){
        stream >> tempFilePath;
        textInformation->append(/*netPath+*/tempFilePath + "\n");
    }

    emit buttonSend->clicked();
}

void Server::mousePress(QDataStream &stream)
{
    QPoint position;
    stream >> position;
    QCursor::setPos(QCursor::pos()+position);
}

void Server::slotTextChange(){

    if(!textInformation->toPlainText().isEmpty() &&
        textInformation->toPlainText()[textInformation->toPlainText().length()-1] != '\n')
    {
        textInformation->append("\n");
    }

}

void Server::setCorrectTextView(){

    QString tmp = textInformation->toPlainText();
    QString correct;

    for(int i = 0; i < tmp.length()-1; i++){
        if(tmp[i] == '\n' && tmp[i+1] == '\n')
            continue;
        else correct += tmp[i];
    }

    correct += tmp[tmp.length()-1];

    textInformation->setPlainText(correct);

}

void Server::slotSendFiles(){

    linePort->setEnabled(false);

    setDefaultSetting();

    labelStatus->setText("Побудова списку папок та файлів");

    createListFiles();

    textInformation->clear();

    { // надіслати клієнту кількість файлів
        QByteArray  array;
        QDataStream arrayStream(&array, QIODevice::WriteOnly);

        arrayStream << quint16(QString::number(stringFilesSend.size()).length())
                    << qint8(Command::COUNT_FILES)
                    << QString::number(stringFilesSend.size());

        Sockets[Command::FILE_MANAGER]->write(array);
    }
    //slotSendFile();

}

void Server::createListFiles(){

    QString tmp = textInformation->toPlainText();
    QStringList tmpList;

    QString currentFile;

    // построение списка
    for(int i = 0; i < tmp.length(); i++){

        if(tmp[i] == '\n'){

            tmpList.push_back(currentFile);
            currentFile.clear();
            continue;
        }

        currentFile += tmp[i];
    }

    // очистка от превикса file:// если он есть
    for(int i = 0; i < tmpList.size(); i++)
        if(tmpList[i].left(8) == "file:///")
            tmpList[i] = tmpList[i].right(tmpList[i].length()-7);

    textInformation->clear();
    foreach(QString str, tmpList)
        textInformation->append(str);

    foreach (QString path, tmpList)
        createListDirAndFiles(path, path.section('/', -1)+"/");

}

void Server::createListDirAndFiles(QString path, QString parent){

    if(parent == "/")
        return;

    QFileInfo info(path+"/");

    if(info.isDir()){

        stringDirections.push_back(parent);

        QFileInfoList list = info.dir().entryInfoList();

        foreach(QFileInfo tmp, list){

            QString filePath = tmp.filePath();
            QString fileName = tmp.fileName();

            if(filePath[filePath.length()-1] == '.')
                 continue;

            if(QFileInfo(filePath).isFile()){
                stringFilesSend.push_back(parent+fileName);
                stringAbsoluteFilesSend.push_back(path+"/"+(parent+fileName).section('/', -1));
            }

            else createListDirAndFiles(filePath, parent+fileName+"/");
        }
    }

    else if(QFileInfo(path).isFile()){

        stringFilesSend.push_back(parent.left(parent.length()-1));
        stringAbsoluteFilesSend.push_back(path);

    }

}

void Server::setDefaultSetting(){

    stringAbsoluteFilesSend.clear();
    stringDirections.clear();
    stringFilesSend.clear();

    isCreatedDirs = false;
    indexDir  = 0;
    indexFile = 0;

    labelStatus->clear();

    setWindowTitle("Сервер");

}

void Server::slotSendFile(){

    quint16 maxBlock(1024*63);

    if(fileSend->pos() == fileSend->size() || !fileSend->isOpen()){
        slotOpenNextFile();
        return;
    }

    QByteArray  blockFile;
    QDataStream blockFileStream(&blockFile, QIODevice::WriteOnly);

    if((fileSend->size() - fileSend->pos()) < maxBlock)
        maxBlock = (quint16)(fileSend->size() - fileSend->pos());

    blockFileStream << (quint16)(maxBlock+1) << qint8(Command::FILE_DATA) << fileSend->read(maxBlock);

    Sockets[Command::FILE_MANAGER]->write(blockFile);

}

void Server::slotNewConnection(){

    buttonSend->setEnabled(true);

    QTcpServer *server = ((QTcpServer*)sender());
    tcpClient = server->nextPendingConnection();

    connect(tcpClient, SIGNAL(readyRead()),    this, SLOT(slotReadyRead()));
    connect(tcpClient, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));

}

void Server::sendNextDir(){

    if(!isCreatedDirs){

        if(indexDir == stringDirections.size()-1 || stringDirections.isEmpty()){

            isCreatedDirs = true;

            if(stringDirections.isEmpty()){
                slotOpenNextFile();
                return;
            }
        }

        QByteArray  data;
        QDataStream stream(&data, QIODevice::WriteOnly);

        stream << quint16(stringDirections[indexDir].length() + sizeof(qint8))
               << qint8(Command::DIR_NAME)
               << stringDirections[indexDir];

        Sockets[Command::FILE_MANAGER]->write(data);
        Sockets[Command::FILE_MANAGER]->flush();

        indexDir++;
    }

    else slotOpenNextFile();

}

void Server::slotReadyRead(){

    QTcpSocket* client = (QTcpSocket*)(sender());

    QDataStream stream(client);

    qint8 data;

    blockFile = 0;

    while(true){

        if(!blockFile){

            if(client->bytesAvailable() < sizeof(quint16))
                break;

            stream >> blockFile;
        }

        if(client->bytesAvailable() < blockFile)
            break;

        stream >> data;
        blockFile = 0;

        qDebug() << "serverread: " << data;

        if (data == (qint8)-96)
            QMessageBox::information(0, "", "MEvent");

        switch (data) {

            case Command::TERMINAL:         terminal();                 break;
            case Command::MANAGER:          connectManager();           break;
            case Command::TERMINAL_COMMAND: terminalCommand(stream);    break;
            case Command::FILE_MANAGER:     fileManager();              break;
            case Command::LIST_FILE_DIR:    listFileDir(stream);        break;
            case Command::GET_FILES:        getFiles(stream);           break;
            //case CManager::Images:          manager->getImages(stream); break; // in pcmanager
            case CManager::GetScreen:       manager->slotSendDisplay(client); break;
            #if plfrm == linos
            case ENMouse::MouseEvent:       manager->mouse.mouseEvent(stream); break;
            #endif
            default : {

                if(!isCreatedDirs)
                    sendNextDir();

                else slotSendFile();
            }
        }
    }


}

void Server::slotDisconnected(){

    buttonSend->setEnabled(false);

    tcpClient->disconnect();
    tcpClient->close();
    tcpClient->deleteLater();
    tcpClient = nullptr;

}

QString Server::getFormatStr(const QString &str, int maxSymbols){

    return (str.length() <= maxSymbols ? str : "..." + str.right(maxSymbols));

}

void Server::slotOpenNextFile(){

    if( fileSend->isOpen()){
        fileSend->flush();
        fileSend->close();
    }

    if(indexFile >= stringFilesSend.size()){

        {
            QByteArray  array;
            QDataStream arrayStream(&array, QIODevice::WriteOnly);
            arrayStream << quint16(sizeof(qint8)) << qint8(Command::FINISH);
            Sockets[Command::FILE_MANAGER]->write(array);
        }

        fileSend->close();

//        QMessageBox::information(nullptr, "Надсилання завершено", "Надіслано: " +
//                                 QString::number(stringFilesSend.size()) +
//                                 " файлів.");

        linePort->setEnabled(true);

        setDefaultSetting();

        return;
    }

    fileSend->setFileName(stringAbsoluteFilesSend[indexFile]);

    if(!fileSend->open(QIODevice::ReadOnly)){
        QMessageBox::critical(nullptr,
                              "Помилка",
                              "Неможливо відкрити файл для зчитування...\n" +
                              stringAbsoluteFilesSend[indexFile]);
        return;
    }

    labelStatus->setText("Надсилаю: " + getFormatStr(stringAbsoluteFilesSend[indexFile], 50));

    setWindowTitle("Сервер – " +
                   QString::number(indexFile+1) + " / " +
                   QString::number(stringFilesSend.size()));

    QByteArray  blockFile;
    QDataStream blockFileStream(&blockFile, QIODevice::WriteOnly);

    blockFileStream << quint16(stringFilesSend[indexFile].size() + sizeof(qint8))
                    << qint8(OPEN_NEXT_FILE)
                    << stringFilesSend[indexFile];

    Sockets[Command::FILE_MANAGER]->write(blockFile);

    indexFile++;

}

void Server::Disconnected(){

    if(tcpClient){
        QByteArray  array;
        QDataStream arrayStream(&array, QIODevice::WriteOnly);
        arrayStream << quint16(sizeof(qint8)) << qint8(Command::DISCONNECT);
        tcpClient->write(array);
    }
    tcpServer->close();
}
