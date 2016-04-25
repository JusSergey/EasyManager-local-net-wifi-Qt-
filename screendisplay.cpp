#include "screendisplay.h"

ScreenDisplay::ScreenDisplay(QGraphicsScene *scene, int countRows/*16*/, int countColumns/*24*/)// : QObject()
{

    this->countColumns = countColumns;
    this->countRows    = countRows;

    Pixmaps.resize(this->countRows);
    Items.resize(this->countRows);

    for(int i = 0; i < this->countRows; i++){
        Pixmaps[i].resize(this->countColumns);
        Items[i].resize(this->countColumns);
    }

    wDesktop = QApplication::desktop()->width();
    hDesktop = QApplication::desktop()->height();



    wScr = wDesktop / this->countColumns;
    hScr = hDesktop / this->countRows;

    updateScreen();

    if(scene)
        setScene(scene);

}

void ScreenDisplay::updateScreen(){

//#if plfrm != andros
    QPixmap screen = QPixmap::grabWindow(QApplication::desktop()->winId());
//#endif
    for(quint16 r = 0; r < countRows; r++){
        for(quint16 c = 0; c < countColumns; c++){
//#if plfrm != andros
            Pixmaps[r][c].pixmap = screen.copy(r*wScr, c*hScr, wScr, hScr);
//#else
//            Pixmaps[r][c].pixmap = QPixmap(wScr, hScr);
//#endif
            Pixmaps[r][c].indexColumn = c;
            Pixmaps[r][c].indexRow    = r;

        }
    }

}

void ScreenDisplay::setScene(QGraphicsScene *scene, float scale){

    for(int r = 0; r < countRows; r++)
        for(int c = 0; c < countColumns; c++)
            Items[r][c] = scene->addPixmap(Pixmaps[r][c].pixmap);

    setScale(scale);

}

void ScreenDisplay::setScale(float scale){

    for(int r = 0; r < countRows; r++){
        for(int c = 0; c < countColumns; c++){
            Items[r][c]->setScale(scale);
            Items[r][c]->setPos(r*wScr*scale, c*hScr*scale);
        }
    }

}

void ScreenDisplay::updateScene(){

    for(int r = 0; r < countRows; r++)
        for(int c = 0; c < countColumns; c++)
            Items[r][c]->setPixmap(Pixmaps[r][c].pixmap);

}

//

void ScreenDisplay::operator >> (QDataStream &stream){

    stream << quint16(0) // size package
           << (qint8)CManager::Images
           << quint16(countColumns*countRows);

//    //stream << Pixmaps[0][0].pixmap;

//    QBuffer buffer;

//    QImageWriter writer(&buffer, "png");

//    writer.setQuality(80);

//    writer.write(Pixmaps[0][0].pixmap.toImage());

//    stream //<< indexRow
//           //<< indexColumn
//           //<< pixmap;
//           << buffer.data();

//    qDebug() << "spix";

    for(int r = 0; r < countRows; r++)
        for(int c = 0; c < countColumns; c++)
            Pixmaps[r][c] >> stream;

}

void ScreenDisplay::operator << (QDataStream &stream){

//    QByteArray array;
//    stream >> array;
    //Items[0][0]->setPixmap(pix);


  //  QByteArray array;

    //stream >> pixmap;


//    QBuffer buffer(&array);

//    buffer.open(QIODevice::ReadOnly);

//    QImageReader reader(&buffer, "png");

//    Items[0][0]->setPixmap(QPixmap::fromImage(reader.read()));

//    qDebug() << "Reader";

    //pixmap = QPixmap::fromImage(reader.read());


    quint16 length;
    stream >> length;

    for(quint16 i = 0; i < length; i++){

        qDebug() << "_operator << " ;

        quint16 indexRows;
        quint16 indexColumn;

        stream >> indexRows >> indexColumn;


        wrlog(("len: " + QString::number(length) + " " + QString::number(indexRows) + " " + QString::number(indexColumn)));

        qDebug() << "Read index's: " << QPoint(indexRows, indexColumn);

        Pixmaps[indexRows][indexColumn] << stream;

        qDebug() << "read pix ";

        Items[indexRows][indexColumn]->setPixmap(Pixmaps[indexRows][indexColumn].pixmap);

    }

}
