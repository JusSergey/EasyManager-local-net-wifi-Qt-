#include "terminal.h"

Terminal::Terminal(QWidget *parent) : QWidget(parent)
{

    blockFile = 0;

    socket = new QTcpSocket(this);

    layout         = new QVBoxLayout;
    buttonCommand  = new MyToolButton("Command", "#640064", "#DDCCCC",this);
    textInfo       = new QTextEdit("terminal\n", this);
    lineCommand    = new QLineEdit(this);

    buttonConnect  = new MyToolButton("Connect", "#640064", "#DDCCCC",this);

    labelPort   = new QLabel("Port\t", this);
    labelIP     = new QLabel("IP\t", this);

    linePort    = new QLineEdit("2323", this);
    lineIp      = new QLineEdit("127.0.0.1", this);

    layoutIP    = new QHBoxLayout;
    layoutPort  = new QHBoxLayout;

    addWidgets(layoutIP, {labelIP, lineIp, buttonConnect});
    addWidgets(layoutPort, {labelPort, linePort});

    textInfo->setReadOnly(true);

    HLayout = new QHBoxLayout;


    addWidgets(HLayout, {lineCommand, buttonCommand});
    layout->addLayout(layoutPort);
    layout->addLayout(layoutIP);
//    layout->addLayout(HLayout);
//    layout->addWidget(textInfo);


    QGroupBox *group = new QGroupBox(this);
    QVBoxLayout *groupLayout = new QVBoxLayout;
    groupLayout->addLayout(HLayout);
    groupLayout->addWidget(textInfo);
    group->setLayout(groupLayout);

    layout->addWidget(group);

    setLayout(layout);

    connect(buttonConnect, SIGNAL(clicked(bool)), this, SLOT(slotConnect()));
    connect(buttonCommand, SIGNAL(clicked(bool)), this, SLOT(slotCommand()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));

    textInfo->setStyleSheet("background-color: #150121; color: #C492F2");
    lineCommand->setStyleSheet("background-color: #190227; color: #C492F2");
    linePort->setStyleSheet("background-color: #190227; color: #C492F2");
    lineIp->setStyleSheet("background-color: #190227; color: #C492F2");

//    buttonCommand->setStyleSheet("background-color: #640064; color: #DDCCCC");
//    buttonConnect->setStyleSheet("background-color: #640064; color: #DDCCCC");

    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this);
        //shadowEffect -> setBlurRadius(26.0);
        shadowEffect -> setColor(QColor(0, 0, 0, 140));
        //shadowEffect -> setOffset(9.0);

//        this->setGraphicsEffect(shadowEffect);

    QPalette palette;



//    palette.setColor(QPalette::Button, QColor(10, 110, 100));
//    buttonCommand->setPalette(palette);

    buttonConnect->setStyleSheet("background: #FF33AA");


        QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect;
        effect->setOpacity(0.8);

        setOpasityWidgets(effect, {lineCommand, lineIp, linePort, textInfo});


}

Terminal::~Terminal()
{
    delete layout;
    delete HLayout;
}

void Terminal::slotConnect()
{
    socket->connectToHost(lineIp->text(), quint16(linePort->text().toInt()));
//    usleep(2000000);

    QByteArray array;
    QDataStream stream(&array, QIODevice::WriteOnly);

    stream << quint16(1) << qint8(Command::TERMINAL);

    socket->write(array);
}

void Terminal::slotCommand()
{
    QByteArray array;
    QDataStream stream(&array, QIODevice::WriteOnly);

    textInfo->setFocus();

    stream << quint16(lineCommand->text().length()+sizeof(Command::TERMINAL_COMMAND))
           << qint8(Command::TERMINAL_COMMAND)
           << lineCommand->text();

    socket->write(array);
}

void Terminal::slotReadyRead()
{
    QDataStream stream(socket);

    blockFile = 0;

    while(true){

        if(!blockFile){

            if(socket->bytesAvailable() < sizeof(quint16))
                break;

            stream >> blockFile;

        }

        if(socket->bytesAvailable() < blockFile)
            break;

        qint8 type;

        stream >> type;

        switch (type) {

            case TERMINAL_COMMAND : {

                QString hostName;
                QByteArray data;
                stream >> hostName >> data;
                textInfo->append(hostName + ":~ > " + lineCommand->text() +'\n' + data.data());

            } break;

        }

    }
}

