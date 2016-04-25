#include "filemanager.h"

FileManager::FileManager(quint16 port, QWidget *parent)
    : QWidget(parent), blockFile(0)
{



    labelSpeed.setText("0");

    flagSelect = false;

    tcpSocket = new QTcpSocket(this);

    statusSend      = true;

    stringStandartPath = QApplication::applicationDirPath() + "/";

    lineIp          = new QLineEdit     ("127.0.0.1",            this);
    linePort        = new QLineEdit     (QString::number(port),  this);
    labelStatus     = new QLabel        (                        this);
    buttonConnect   = new MyToolButton  ("&З'єднатися" ,   "#190227", "#C492F2", this);
    buttonPathDir   = new MyToolButton  ("&Вибрати папку", "#190227", "#C492F2", this);
    buttonPause     = new MyToolButton  ("&Пауза", "#190227", "#C492F2", this);
    buttonSelect    = new MyToolButton  ("Select", "#190227", "#C492F2", this);
    buttonSend      = new MyToolButton  ("Send",   "#190227", "#C492F2", this);
    buttonClear     = new MyToolButton  ("Clear",  "#190227", "#C492F2", this);

    senderWidget = new MyListWidget(this);
    connect(senderWidget, SIGNAL(clicked(QModelIndex)), senderWidget, SLOT(removeItemWidget(QModelIndex)));
    listWidget = new MyListWidget(this);

    listWidget->addItem(c_listWidgetGet);

    listWidget->setAutoScroll(true);
    QFont font;
    font.setPixelSize(14);
    listWidget->setFont(font);
    senderWidget->setFont(font);

    layoutPort = new QHBoxLayout();
    layoutPort->addWidget(new QLabel("Порт:\t", this));
    layoutPort->addWidget(linePort);

    layoutIp = new QHBoxLayout();
    layoutIp->addWidget(new QLabel("IP:\t", this));
    layoutIp->addWidget(lineIp);

    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->addLayout(layoutPort);
    layout->addLayout(layoutIp);

    QHBoxLayout* horisontal = new QHBoxLayout(this);
    horisontal->addWidget(buttonConnect);
    horisontal->addWidget(buttonPathDir);
    horisontal->addWidget(buttonPause);

    QHBoxLayout* lists = new QHBoxLayout(this);
    lists->addWidget(listWidget);
    lists->addWidget(senderWidget);

    allFiles = new QProgressBar(this);

    allFiles->setMaximumHeight(20);

    layout->addLayout(horisontal);
    QHBoxLayout *hl = new QHBoxLayout(this);
    addWidgets(hl, {buttonSelect, buttonSend, buttonClear});

    layout->addWidget(allFiles);
    layout->addLayout(hl);
    layout->addLayout(lists);
    layout->addWidget(labelStatus);
    layout->addWidget(&labelSpeed);

    setLayout(layout);

    connect(buttonConnect, SIGNAL(clicked(bool)),  this, SLOT(slotButtonConnect()));
    connect(buttonPathDir, SIGNAL(clicked(bool)),  this, SLOT(slotPathDir()));
    connect(buttonPause  , SIGNAL(clicked(bool)),  this, SLOT(slotPause()));
    connect(tcpSocket,     SIGNAL(readyRead()),    this, SLOT(slotReadyRead()));
    connect(listWidget,    SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(slotClickItem(QListWidgetItem*)));
    connect(buttonSend,    SIGNAL(clicked(bool)),  this, SLOT(slotButtonSend()));
    connect(buttonSelect,  SIGNAL(clicked(bool)),  this, SLOT(slotButtonSelect()));
    connect(&timerConnectToServer, SIGNAL(timeout()), this, SLOT(slotConnectToServer()));
    connect(this, SIGNAL(signalGetPackage()), this, SLOT(slotGetPackage()));
    connect(buttonClear, SIGNAL(clicked(bool)), senderWidget, SLOT(slotClear()));
    //connect(this, SIGNAL())

    this->setGeometry(400, 200, 400, 100);

    setWindowTitle("Клієнт");

    buttonPause->setEnabled(false);

    setEnablePauseButton();

    //textInfo->setStyleSheet("background-color: #150121; color: #C492F2");
    lineIp->setStyleSheet("background-color: #190227; color: #C492F2");
    linePort->setStyleSheet("background-color: #190227; color: #C492F2");
    //listWidget->setStyleSheet("background-color: #150121; color: #C492F2");

    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect;
    effect->setOpacity(0.8);

    setOpasityWidgets(effect, {linePort, lineIp, allFiles});

//    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this);
//        shadowEffect -> setBlurRadius(26.0);
//        shadowEffect -> setColor(QColor(0, 0, 0, 140));
//        shadowEffect -> setOffset(9.0);

//        this->setGraphicsEffect(shadowEffect);



}

void FileManager::slotGetPackage(){

    QByteArray array;
    QDataStream stream(&array, QIODevice::WriteOnly);

    stream << quint16(1) << qint8(Command::GET_PACKAGE);

    tcpSocket->write(array);

}

FileManager::~FileManager()
{
    delete layoutIp;
    delete layoutPort;
}

QString FileManager::getFormatStr(const QString &str, int maxSymbols){

    return (str.length() <= maxSymbols ? str : "..." + str.right(maxSymbols));

}

void FileManager::slotPathDir(){

    QString pathDir(QFileDialog::getExistingDirectory());

    if(!pathDir.isEmpty()){
        stringStandartPath = pathDir + "/";
        labelStatus->setText(getFormatStr("Папка: "+pathDir, 51));
    }

}

void FileManager::setEnablePauseButton(){

    if(buttonConnect->text() != "&З'єднано")
         buttonPause->setEnabled(false);

}

void FileManager::resizeEvent(QResizeEvent *event)
{
    int width = event->size().width()-28;

    buttonSelect->setGeometry(listWidget->x(), buttonSelect->y(), width/4, buttonSelect->height());
    buttonSelect->setFixedSize(width/4, buttonSelect->height());

    buttonSend->setGeometry(buttonSelect->x()+buttonSelect->width()+5, buttonSend->y(), width/4, buttonSend->height());
    buttonSend->setFixedSize(width/4, buttonSend->height());

    buttonConnect->setGeometry(listWidget->x(), buttonConnect->y(), width/3, buttonConnect->height());
    buttonConnect->setFixedSize(width/3, buttonConnect->height());

    buttonPathDir->setGeometry(buttonConnect->x()+buttonConnect->width()+5, buttonConnect->y(), width/3, buttonPathDir->height());
    buttonPathDir->setFixedSize(width/3, buttonPathDir->height());

    buttonPause->setGeometry(buttonPathDir->x()+buttonPathDir->width()+5, buttonPause->y(), width/3, buttonPause->height());
    buttonPause->setFixedSize(width/3, buttonPause->height());

    buttonClear->setGeometry(senderWidget->x(), buttonClear->y(), senderWidget->width(), buttonClear->height());
    buttonClear->setFixedSize(senderWidget->width()-5, buttonClear->height());

}

void FileManager::slotReadyRead(){

    QDataStream stream(tcpSocket);

    qint8 command(0);

    while(true){

        if(!blockFile){

            if(tcpSocket->bytesAvailable() < sizeof(quint16))
                break;

            stream >> blockFile;

        }

        if((tcpSocket->bytesAvailable() < blockFile))
            break;

        if(!command)
            stream >> command;

        static int vl(0), q(0);
        vl += tcpSocket->bytesAvailable();

        if(!(q++ % 20))
            labelSpeed.setText(QString::number(double(vl)/1024));

        switch (command) {

            case FILE_DATA:         {slotFileData(stream);      blockFile = 0; emit signalGetPackage(); return;}
            case OPEN_NEXT_FILE:    {slotOpenNextFile(stream);  blockFile = 0; emit signalGetPackage(); return;}
            case DIR_NAME:          {slotDirName(stream);       blockFile = 0; emit signalGetPackage(); return;}
            case COUNT_FILES:       {slotCountFiles(stream);    blockFile = 0; emit signalGetPackage(); return;}
            case LIST_FILE_DIR:     {slotListFileDir(stream);    break;}
            case FINISH:            {slotFinish();              blockFile = 0; return;}
            case CONNECTED:         {slotConnect();             blockFile = 0; return;}
            case DISCONNECT:        {slotConnectToServer(true); blockFile = 0; return;}

        }
        if(tcpSocket->bytesAvailable() == 0) return;

    }

}

void FileManager::slotPause(){

    if(buttonPause->text() == "&Продовжити"){
        buttonPause->setText("&Пауза");
        statusSend = true;
        slotGetPackage();
    }
    else {
        buttonPause->setText("&Продовжити");
        statusSend = false;
    }

}

void FileManager::slotButtonConnect(){

    slotConnectToServer(true);

    QByteArray array;
    QDataStream stream(&array, QIODevice::WriteOnly);

    stream << quint16(sizeof(Command::FILE_MANAGER)) << qint8(Command::FILE_MANAGER);

    tcpSocket->write(array);

}

void FileManager::slotOpenNextFile(QDataStream &stream)
{

    labelSpeed.setText("0");

    file.flush();
    file.close();

    QString tmpFilePath;
    stream >> tmpFilePath;

    if(tmpFilePath.isEmpty())
        return;

    indexFile++;

    labelStatus->setText("Отримую: " + getFormatStr(stringStandartPath + tmpFilePath, 51));

    file.setFileName(stringStandartPath + tmpFilePath);
    file.remove();

    allFiles->setValue(int(100.0/((float)valueFiles/(float)indexFile)));

    setWindowTitle("Клієнт – " + QString::number(indexFile)+" / "+QString::number(valueFiles));

    slotFileData(stream);

}

void FileManager::slotFileData(QDataStream &stream)
{
    QByteArray data;

    if(!file.open(QIODevice::Append))
        QMessageBox::critical(nullptr, "Error", "Can't open file Append: " + file.fileName());

    stream >> data;

    blockFile = 0;
    file.write(data);
    file.flush();
    file.close();



}

void FileManager::slotCountFiles(QDataStream &stream)
{

    QString stringValueFiles;
    stream >> stringValueFiles;

    indexFile   = 0;
    valueFiles  = stringValueFiles.toInt();
}

void FileManager::slotDirName(QDataStream &stream)
{

    buttonPause->setEnabled(true);

    lineIp->setEnabled(false);
    linePort->setEnabled(false);

    QString pathToDir;
    stream >> pathToDir;

    QDir().mkdir(stringStandartPath + pathToDir);

}

void FileManager::slotListFileDir(QDataStream &stream)
{

    QByteArray d;
    stream >> d;

    if(d.isEmpty()){
        QMessageBox::information(nullptr, "FUCK!!!!", "IS_EMPTY");
        return;
    }

    blockFile = 0;

    QString datas = d;

//    QFile f("/mnt/sdcard/fuck.txt");
//    f.open(QIODevice::WriteOnly);
//    f.write(datas.toStdString().c_str());
//    f.flush();
//    f.close();

    QStringList list = stringToList(datas);

    listWidget->clear();

    netPath = list.first();

    foreach(QString name, list){
        //listWidget->addItem(name);
        listWidget->add(name);
    }



}

void FileManager::slotConnect()
{
    buttonConnect->setText("&З'єднано");
    setEnablePauseButton();
}

void FileManager::slotFinish()
{

    buttonPause->setEnabled(false);

    lineIp->setEnabled(true);
    linePort->setEnabled(true);

    QMessageBox::information(nullptr, "Отримування файлів завершено", "Отримано: " +
                             QString::number(valueFiles) + " файлів.");
    setWindowTitle("Клієнт");
    labelStatus->setText("Папка: " + getFormatStr(stringStandartPath, 51));

}

void FileManager::slotClickItem(QListWidgetItem *item)
{

    if(flagSelect){
        senders.insert(item->text(), netPath);//  ((MyListWidgetItem*)item)->isFile() ? "F" : "D";
        senderWidget->add(item->text()+(((MyListWidgetItem*)item)->isFile() ? "F" : "D"));
        item->setSelected(true);
    }

    else if(((MyListWidgetItem*)item)->isFile() && ((MyListWidgetItem*)item)->text() != c_listWidgetGet)
        return;

    else {

        QByteArray array;
        QDataStream stream(&array, QIODevice::WriteOnly);

        if(item->text() == ".."){

            int i;
            for(i = netPath.length()-2; i >= 0 && (netPath.toStdString())[i] != '/'; i--);
            netPath.resize(i+1);

        }

        stream << quint16(netPath.length() + item->text().length() + sizeof(qint8))
               << qint8(LIST_FILE_DIR)
               << netPath + (item->text() == ".." ? "" : item->text() + "/");

        tcpSocket->write(array);

    }
}

void FileManager::slotButtonSelect()
{
    flagSelect = !flagSelect;
}

void FileManager::slotButtonSend()
{

    QByteArray array;
    QDataStream stream(&array, QIODevice::WriteOnly);

    QStringList files;

    quint16 allLength(0);

    int len = senderWidget->count();

    for(int i = 0; i < len; i++){
        QString str = senders[senderWidget->item(i)->text()] + senderWidget->item(i)->text();
        files.append(str);
        allLength += str.length();
    }

    stream << quint16(sizeof(quint16/*count items*/) + allLength + netPath.length() + sizeof(qint8))
           << qint8(GET_FILES)
           << quint16(files.size())
           << netPath;

    foreach(QString temp, files)
        stream << temp;

    tcpSocket->write(array);

    senders.clear();

}

void FileManager::slotConnectToServer(bool click){

    QString tmpPort = linePort->text();

    for(int i = 0; i < tmpPort.length(); i++)
        if(tmpPort[i] < '0' || tmpPort[i] > '9')
            return;

    if(buttonConnect->text() == "&З'єднатися" || buttonConnect->text() == "&З'єднання"){

        if(buttonConnect->text() == "&З'єднання"){
            tcpSocket->disconnectFromHost();
            buttonConnect->setText("&З'єднатися");
            return;
        }

        if(click)
            buttonConnect->setText("&З'єднання");

        tcpSocket->connectToHost(lineIp->text(), (quint16)linePort->text().toInt());
        timerConnectToServer.start(1024);
    }

    else if(buttonConnect->text() == "&Від'єднатися" ||
            buttonConnect->text() == "&З'єднання"    ||
            buttonConnect->text() == "&З'єднано")
    {

        if(buttonConnect->text() == "&З'єднано"){
            timerConnectToServer.stop();
        }
        timerConnectToServer.stop();

        if(click){
            tcpSocket->disconnectFromHost();
            buttonConnect->setText("&З'єднатися");
            timerConnectToServer.stop();
        }

    }
    setEnablePauseButton();
    timerConnectToServer.stop();
}
