#include "info.h"
#include "function.h"

Info::Info(QWidget *parent) : QWidget(parent)
{
    labelLocalAddress = new QLabel(this);
    in = new QLabel(this);
    radioFullScreen = new QRadioButton("Full screen", this);
    radioNormalScreen = new QRadioButton("Normal screen", this);

    radioNormalScreen->setChecked(true);

    layout = new QVBoxLayout;

    line = new QLineEdit(this);
    but = new QToolButton(this);
    but->setText("but");

    QList<QHostAddress> tempAddresses = QNetworkInterface::allAddresses();

    QString localAddress;

    in->setText("bad-moon.jpg\n"
                "beutyful-moon.jpg\n"
                "big-moon.jpg\n"
                "cat.jpg\n"
                "christy.jpg\n"
                "house.jpg\n"
                "moon-water.jpg\n"
                "night-moon-water.jpg\n"
                "night-moon.jpg\n"
                "super-night.jpg");

    foreach(QHostAddress address, tempAddresses)
        if(address.toString().left(3) == "192")
            localAddress = address.toString();

    labelLocalAddress->setText(localAddress);

    addWidgets(layout, {radioFullScreen, radioNormalScreen, labelLocalAddress, in, line, but});

    setLayout(layout);

    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this);
        shadowEffect -> setBlurRadius(16.0);
        shadowEffect -> setColor(QColor(0, 0, 0, 140));
        shadowEffect -> setOffset(16.0);

        this->setGraphicsEffect(shadowEffect);

        connect(radioFullScreen, SIGNAL(clicked(bool)), this, SIGNAL(signalShowFullScreen()));
        connect(radioNormalScreen, SIGNAL(clicked(bool)), this, SIGNAL(signalShowNormal()));
        connect(but, SIGNAL(clicked(bool)), this, SLOT(slotEmit()));

}

void Info::slotEmit()
{
    but->setFocus();
    signalSetBackground(line->text());
}

