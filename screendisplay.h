#ifndef SCREENDISPLAY_H
#define SCREENDISPLAY_H
#include <QtWidgets>
#include "constant.h"
#include "function.h"


class ScreenDisplay// : public QObject
{
//    Q_OBJECT

public:
    struct FragmentDisplay {

        quint16 indexRow;
        quint16 indexColumn;
        QPixmap pixmap;

        bool isChange;

        FragmentDisplay(){
            indexRow = indexColumn = 0;
            isChange = false;
        }

        void operator >> (QDataStream &stream){

            QBuffer buffer;

            QImageWriter writer(&buffer, "jpg");

            writer.setQuality(20);

            writer.write(pixmap.toImage());

            stream << indexRow
                   << indexColumn
//                   << pixmap;
                   << buffer.data();

            qDebug() << "spix";

        }

        void operator << (QDataStream &stream){

            QByteArray array;

            stream >> array;

            QBuffer buffer(&array);

            buffer.open(QIODevice::ReadOnly);

            QImageReader reader(&buffer, "jpg");

            pixmap = QPixmap::fromImage(reader.read());

        }

    };

public:
    explicit ScreenDisplay(QGraphicsScene *scene = 0, int countRows = 2, int countColumns = 2);

private:
    QVector<QVector<FragmentDisplay>> Pixmaps;
    QVector<QVector<QGraphicsPixmapItem*>> Items;

private:
    int countRows;
    int countColumns;
    int wDesktop;
    int hDesktop;

    int wScr;
    int hScr;

public:
    void updateScreen();
    void setScene(QGraphicsScene *scene, float scale = 1.0f);
    void updateScene();

    void setScale(float scale);

public:
    void operator >> (QDataStream &stream);
    void operator << (QDataStream &stream);

};

#endif // SCREENDISPLAY_H

