#include "NetwotkManager.h"

NetworkManager::NetworkManager(QWidget *parent)
    : QTabWidget(parent)
{


//    setStyleSheet("{background-color: black; color: #00FF00}");
//    setStyleSheet("{background-color: black; color: #00FF00}");
//    setStyleSheet("{background-color: black; color: #BBBBBB}");
//    setStyleSheet("{background-color: black; color: #BBBBBB}");

    QPalette palette;
//    QBrush brush;

    //brush.setTextureImage(QImage(":/background/house.jpg"));
    palette.setColor(QPalette::Button, QColor(30, 30, 30));
//    palette.setColor(QPalette::Text, QColor(200, 200, 200));
//    palette.setColor(QPalette::WindowText, QColor(200, 200, 200));
    //palette.setColor(QPalette::Foreground, QColor(50, 50, 50));

//    palette.setBrush(QPalette::Window, QBrush(QColor(10, 10, 10)));
    palette.setBrush(QPalette::Base, QBrush(QColor(20, 20, 20)));
    this->setPalette(palette);

   // widgets      = new QTabWidget(this);
    fileManager  = new FileManager(2323, this);
    terminal     = new Terminal(this);
    manager      = new PCManager(this);
    info         = new Info(this);

    server       = new Server(2323, manager, this);

    this->addTab (fileManager, "FileManager");
    this->addTab (server,      "Server");
    this->addTab (terminal,    "Terminal");
    this->addTab (manager,     "Manager");
    this->addTab (info,        "Info");

    this->setCurrentWidget(manager);

    layoutTabs = new QVBoxLayout;

    //layoutTabs->addWidget(widgets);

    setLayout(layoutTabs);

    setGeometry(300, 300, 500, 600);

    connect(info, SIGNAL(signalShowFullScreen()), this, SLOT(slotShowFullScreen()));
    connect(info, SIGNAL(signalShowNormal()), this, SLOT(slotShowNormal()));
    connect(info, SIGNAL(signalSetBackground(QString)), this, SLOT(slotSetBackground(QString)));

}

NetworkManager::~NetworkManager()
{
    delete layoutTabs;
}

void NetworkManager::slotSetBackground(QString path)
{
    QPalette palette;
    QBrush brush;

    brush.setTextureImage(QImage(":/background/"+path));

    palette.setBrush(QPalette::Window, brush);
    palette.setBrush(QPalette::Base, brush);
    this->setPalette(palette);
}
