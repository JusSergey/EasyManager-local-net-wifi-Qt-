#include "function.h"

void addWidgets(QLayout* layout, std::initializer_list <QWidget*> tmp){

    foreach(QWidget* t, tmp)
        layout->addWidget(t);

}

QStringList StringToPaths(const QString &str){

    qDebug() << "ToPaths: " << str;

    QStringList listPaths;

    QString currentPath;

    // построение списка
    for(int i = 0; i < str.length(); i++){

        if(str[i] == '\n'){
            listPaths.push_back(currentPath);
            currentPath.clear();
            continue;
        }

        currentPath += str[i];
    }

    for(int i = 0; i < listPaths.length(); i++)
        if(listPaths[i].left(8) == "file:///")
            listPaths[i] = listPaths[i].right(listPaths[i].length()-7);

    return listPaths;

}

inline QString getFormatStr(const QString& str, int maxSymbols){

    return (str.length() <= maxSymbols ? str : "..." + str.right(maxSymbols));

}

QImage getImage(QRgb rgb, QSize size){

    QPixmap pixmap(size);

    QImage image = pixmap.toImage();

    for(int i = 0; i < size.height(); i++)
        for(int j = 0; j < size.width(); j++)
            image.setPixel(i, j, rgb);

    return image;

}

void setOpasityWidgets(QGraphicsOpacityEffect *effect, std::initializer_list<QWidget *> tmp)
{
    foreach (QWidget *widget, tmp)
        widget->setGraphicsEffect(effect);

}

QStringList stringToList(const QString &str)
{
    QStringList listPaths;

    QString currentPath;

    for(int i = 0; i < str.length(); i++){

        if(str[i] == '\n'){
          //  QMessageBox::information(nullptr, "_12345__", currentPath);
            listPaths.push_back(currentPath);
            currentPath.clear();
            continue;
        }

        currentPath += str[i];
    }
    return listPaths;
}

void wrlog(const QString &string) {

#if plfrm == andros
    const QString path = "/mnt/sdcard/log.txt";
#else
    const QString path = "log.txt";
#endif

    static QFile file(path);

    if(!file.isOpen())
        if(!file.open(QIODevice::WriteOnly))
            return;

    file.write(string.toStdString().c_str());
    file.write("\n");

}
