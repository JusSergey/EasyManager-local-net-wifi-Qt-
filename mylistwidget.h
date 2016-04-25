#ifndef MYLISTWIDGET_H
#define MYLISTWIDGET_H

#include <QtWidgets>

class MyListWidget;

class MyListWidgetItem : public QListWidgetItem {

public:
    MyListWidgetItem (const QString &text = "", QListWidget *parent = 0) : QListWidgetItem(text.left(text.length()-1), parent){
        qDebug() << "gt: " << text;
        text.right(1) == "F" ? isFile_ = true : isFile_ = false;
    }

    inline bool isFile() const { return isFile_; }

private:
    bool isFile_;

    friend class MyListWidget;

};

class MyListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit MyListWidget(QWidget *parent = 0);

public slots:
    void add(QString text);

private slots:
    void slotClick();
    void removeItemWidget(QModelIndex);

public slots:
    void slotClear();

signals:
    void signalGetList(QString path);

};

#endif // MYLISTWIDGET_H
