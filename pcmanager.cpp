#include "pcmanager.h"

PCManager::PCManager(QWidget *parent) : QWidget(parent)
{

    setMouseTracking(true);

    is = true;

    {
        socket   = new QTcpSocket(this);
        scene    = new QGraphicsScene(this);
        view     = new QGraphicsView(scene, this);

        timerUpdateDisplay    = new QTimer(this);

        lineIp   = new QLineEdit("127.0.0.1", this);
        linePort = new QLineEdit("2323", this);

        buttonConnect = new MyToolButton  ("&Connect", "#190227", "#C492F2", this);

        lip     = new QHBoxLayout;
        lport   = new QHBoxLayout;
        layoutScale = new QHBoxLayout;
        layout  = new QVBoxLayout;

        sliderScale = new QSlider(Qt::Horizontal, this);
        labelScale  = new QLabel("1.0", this);
        sliderScale->setPageStep(1);
        sliderScale->setMaximum(50);
        sliderScale->setMinimum(1);
        sliderScale->setValue(10);

        addWidgets(layoutScale, {sliderScale, labelScale});

        stream = new QDataStream(&array, QIODevice::WriteOnly);

        SDisplay = new ScreenDisplay(scene);

    }

    wrlog("log starting...");

    labelIp.setText("IP:\t");
    labelPort.setText("Port:\t");

    addWidgets(lip, {&labelIp, lineIp, buttonConnect});
    addWidgets(lport, {&labelPort, linePort});

//    view->setSceneRect(0, 0, 400, 400);
    layout->addLayout(lport);
    layout->addLayout(lip);
    layout->addLayout(layoutScale);
    layout->addWidget(view);

    setLayout(layout);

    timeMsec = 50;

    movedPoint = pressPoint = QCursor::pos();

    {
        //    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this);
        //    shadowEffect -> setBlurRadius(26.0);
        //    shadowEffect -> setColor(QColor(0, 0, 0, 140));
        //    shadowEffect -> setOffset(9.0);

        //    this->setGraphicsEffect(shadowEffect);

           connect(buttonConnect, SIGNAL(clicked(bool)), this, SLOT(slotConnect()));

    }

    connect(timerUpdateDisplay, SIGNAL(timeout()), this, SLOT(slotGetScreen()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(sliderScale, SIGNAL(valueChanged(int)), this, SLOT(slotSliderChande(int)));

}

void PCManager::getImages(QDataStream &stream){

    *SDisplay << stream;
}

void PCManager::mouseMoveEvent(QMouseEvent *event){

    Q_UNUSED(event);

    if(!timerUpdateDisplay->isActive())
        return;

    static QTime previousTime(QTime::currentTime());

    if(toMSec(QTime::currentTime(), previousTime) > timeMsec){

        QByteArray a;
        QDataStream s(&a, QIODevice::WriteOnly);

        s << quint16(sizeof(QPoint) + sizeof(qint8)*2)
          << qint8(ENMouse::MouseEvent)
          << qint8(ENMouse::Move)
          << (QCursor::pos());

      //  socket->write(a);

        pressPoint = QCursor::pos();

    }

}

void PCManager::mousePressEvent(QMouseEvent *event){

    pressPoint = QCursor::pos();

}


void PCManager::mouseReleaseEvent(QMouseEvent *event){

    if(!timerUpdateDisplay->isActive())
        return;

    Q_UNUSED(event)


    QByteArray a;
    QDataStream s(&a, QIODevice::WriteOnly);

    s << quint16(sizeof(qint8)*2)
            << qint8(ENMouse::MouseEvent)
            << qint8(ENMouse::LRelease);

     //slotSend();

    //socket->write(a);

}

void PCManager::mouseDoubleClickEvent(QMouseEvent *event)
{

}

void PCManager::keyPressEvent(QKeyEvent *key)
{
//    SDisplay->updateScreen();
//    SDisplay->updateScene();
}

void PCManager::slotConnect(){

    buttonConnect->setText("Connected");

    socket->connectToHost(lineIp->text(), quint16(linePort->text().toUShort()));

    timerUpdateDisplay->start(100);
}

void PCManager::slotGetScreen(){

    static const char data[] = {'\0', 0b00000001, (qint8)CManager::GetScreen};

    socket->write(data, 3);
    qDebug() << "GET_SCREEN";

}

void PCManager::slotSliderChande(int value){

    labelScale->setText(QString::number(float(value) / 10.0f));
    SDisplay->setScale(float(value) / 10.0f);

}

void PCManager::slotReadyRead()
{
    QTcpSocket* client = (QTcpSocket*)(sender());

    QDataStream stream(client);

    qint8 data;

    static quint16 blockFile(0);

    while(true){

        if(!blockFile){

            if(client->bytesAvailable() < sizeof(quint16))
                break;

            stream >> blockFile;
        }

        if(client->bytesAvailable() < blockFile)
            break;

        stream >> data;


        if(data = (qint8)CManager::Images){
            qDebug() << blockFile << " bytes";
            getImages(stream);
            blockFile = 0;
     //       client->readAll();
        }
        else {
            QMessageBox::critical(0, "...", "Fail");
        }

    }

}

void PCManager::slotSendDisplay(QTcpSocket *client){

    QByteArray bytes;
    QDataStream s(&bytes, QIODevice::WriteOnly);

    SDisplay->updateScreen();

    *SDisplay >> s;

    s.device()->seek(0);
    //s << quint16(65534);
    s << quint16(bytes.size() - sizeof(quint16));

    qDebug() << "size aval: " << bytes.size()/1024;

    client->write(bytes);

}
