#ifndef FUNCTION
#define FUNCTION
#include <QtWidgets>
#include "constant.h"

void addWidgets(QLayout* layout, std::initializer_list <QWidget*> tmp);
QStringList StringToPaths(const QString& str);
inline QString getFormatStr(const QString& str, int maxSymbols);
QImage getImage(QRgb rgb = Qt::black, QSize size = QSize(5, 5));
void setOpasityWidgets(QGraphicsOpacityEffect *effect, std::initializer_list<QWidget*> tmp);
QStringList stringToList(const QString& str);
void wrlog(const QString &string);

#endif // FUNCTION

