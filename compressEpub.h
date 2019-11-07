#ifndef COMPRESSEPUB_H
#define COMPRESSEPUB_H

#include <QObject>
#include <QtCore>
#include "JlCompress.h"

class compressEpub : public QObject
{
    Q_OBJECT
public:
    explicit compressEpub(QObject *parent = 0);
    static bool compressDir(QString dirPath, QString epubPath);
    static bool compressWithMaterials(QString dirPath, QString epubPath, QStringList fileList);
    static bool compressArchive(QString dirPath, QString zipPath, QString parentName = nullptr);

private:
    static QStringList getAllEpubFiles(QString dirPath, QStringList list = QStringList());
};

#endif // COMPRESSEPUB_H
