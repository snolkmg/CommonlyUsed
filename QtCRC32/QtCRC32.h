#ifndef QTCRC32_H
#define QTCRC32_H

#include <QObject>
#include "CRC.h"
#include <QTextStream>
#include <QFile>

class QtCRC32 : public QObject
{
    Q_OBJECT
public:
    explicit QtCRC32(QObject *parent = 0);
    static QString CRCString(QString text, bool isLine = true, bool upper = true);
    static QString CRCFile(QString filePath, bool isLine = true, bool upper = true);

private:
    static QString CRCAllString(QString text, bool upper = true);
    static QString CRCLineString(QString text, bool upper = true);
    static QString CRCAllFile(QString filePath, bool upper = true);
    static QString CRCLineFile(QString filePath, bool upper = true);
};

#endif // QTCRC32_H
