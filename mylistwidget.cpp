#include "mylistwidget.h"

MyListWidget::MyListWidget(QWidget *parent) : QListWidget(parent)
{
    connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(slotClick()));
}

void MyListWidget::add(QString text)
{
    if(text.length() < 2 || text == ".D")
        return;

    static quint64 i(0);
    static QFont font;

    font.setPixelSize(24);

    static QColor color[] = {QColor(30, 30, 30, 100), QColor(40, 40, 40, 100)};

    MyListWidgetItem *item(new MyListWidgetItem(text, this));

    item->setBackground(QBrush(color[i++%2]));
    item->setSizeHint(QSize(item->sizeHint().width(), 40));
    item->setFont(font);

    item->setIcon(item->isFile() ? QIcon(":/icon/file.ico") : QIcon(":/icon/folder.ico"));

}

void MyListWidget::slotClick()
{
    emit signalGetList(this->selectedItems().at(0)->text()+"/");
}

void MyListWidget::removeItemWidget(QModelIndex item)
{
    delete takeItem(item.row());
}

void MyListWidget::slotClear()
{
    this->clear();
}

