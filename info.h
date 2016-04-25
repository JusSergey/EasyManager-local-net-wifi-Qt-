#ifndef INFO_H
#define INFO_H

#include <QtWidgets>
#include <QtNetwork>

class Info : public QWidget
{
    Q_OBJECT
public:
    explicit Info(QWidget *parent = 0);

private:
    QLabel *labelLocalAddress;
    QLabel *in;
    QLineEdit *line;
    QToolButton *but;

private:
    QVBoxLayout *layout;
    QRadioButton *radioFullScreen;
    QRadioButton *radioNormalScreen;

signals:
    void signalShowFullScreen();
    void signalShowNormal();
    void signalSetBackground(QString path);


private slots:
    void slotEmit();

};

#endif // INFO_H
