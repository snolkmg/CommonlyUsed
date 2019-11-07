#include "compressEpub.h"

compressEpub::compressEpub(QObject *parent)
    : QObject(parent)
{

}

QStringList compressEpub::getAllEpubFiles(QString dirPath, QStringList list)
{
    QDir dir(dirPath);
    QFileInfoList info_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot
                                                | QDir::NoSymLinks | QDir::AllDirs);
    foreach(QFileInfo file_info, info_list) {
        if(file_info.isDir())
            list = getAllEpubFiles(file_info.absoluteFilePath(), list);
        else if(file_info.isFile()) {
            if(file_info.fileName() == "mimetype")
                list.prepend(file_info.absoluteFilePath());
            else
                list << file_info.absoluteFilePath();
        }
    }
    return list;
}

bool compressEpub::compressDir(QString dirPath, QString epubPath)
{
    QStringList list = getAllEpubFiles(dirPath);
    QuaZip zip(epubPath);
    if(!zip.open(QuaZip::mdCreate)){
        qCritical() << QString("%1 创建失败！").arg(epubPath);
        return false;
    }
    QuaZipFile zipFile(&zip);
    for(int i = 0; i < list.size(); i++) {
        QFileInfo info(list.at(i));
        if(info.exists()) {
            QString filePath = info.filePath();
            QString fileName = filePath;
            fileName = fileName.replace(dirPath + "/", "");
            QFile file(filePath);
            if(!file.open(QIODevice::ReadOnly)) {
                qCritical() << QString("%1 文件读取失败！").arg(filePath);
                return false;
            }
            if(filePath.endsWith("mimetype")) {
                if(!zipFile.open(QuaZipFile::WriteOnly, QuaZipNewInfo(fileName, filePath), NULL, 0, 0, 0)) {
                    qCritical() << QString("%1 文件写入失败！").arg(filePath);
                    return false;
                }
            } else {
                if(!zipFile.open(QuaZipFile::WriteOnly, QuaZipNewInfo(fileName, filePath))) {
                    qCritical() << QString("%1 文件写入失败！").arg(filePath);
                    return false;
                }
            }
            zipFile.write(file.readAll());
            zipFile.close();
            file.close();
        } else {
            qCritical() << QString("%1 文件不存在！").arg(list.at(i));
            return false;
        }
    }
    zip.close();
    return true;
}

bool compressEpub::compressWithMaterials(QString dirPath, QString epubPath, QStringList fileList)
{
    QStringList list = getAllEpubFiles(dirPath);
    QuaZip zip(epubPath);
    if(!zip.open(QuaZip::mdCreate)){
        qCritical() << QString("%1 创建失败！").arg(epubPath);
        return false;
    }
    QuaZipFile zipFile(&zip);
    for(int i = 0; i < list.size(); i++) {
        QFileInfo info(list.at(i));
        if(info.exists()) {
            QString filePath = info.filePath();
            QString fileName = filePath;
            fileName = fileName.replace(dirPath + "/", "");
            QFile file(filePath);
            if(!file.open(QIODevice::ReadOnly)) {
                qCritical() << QString("%1 文件读取失败！").arg(filePath);
                return false;
            }
            if(filePath.endsWith("mimetype")) {
                if(!zipFile.open(QuaZipFile::WriteOnly, QuaZipNewInfo(fileName, filePath), NULL, 0, 0, 0)) {
                    qCritical() << QString("%1 文件写入失败！").arg(filePath);
                    return false;
                }
            } else {
                if(!zipFile.open(QuaZipFile::WriteOnly, QuaZipNewInfo(fileName, filePath))) {
                    qCritical() << QString("%1 文件写入失败！").arg(filePath);
                    return false;
                }
            }
            zipFile.write(file.readAll());
            zipFile.close();
            file.close();
        } else {
            qCritical() << QString("%1 文件不存在！").arg(list.at(i));
            return false;
        }
    }

    for(int i = 0; i < fileList.size(); i++) {
        QFileInfo info(fileList.at(i));
        if(info.exists()) {
            QString filePath = info.filePath();
            QString fileName = info.fileName();
            fileName = QString("OEBPS/Images/%1").arg(fileName);
            QFile file(filePath);
            if(!file.open(QIODevice::ReadOnly)) {
                qCritical() << QString("%1 文件读取失败！").arg(filePath);
                return false;
            }
            if(!zipFile.open(QuaZipFile::WriteOnly, QuaZipNewInfo(fileName, filePath))) {
                qCritical() << QString("%1 文件写入失败！").arg(filePath);
                return false;
            }
            zipFile.write(file.readAll());
            zipFile.close();
            file.close();
        } else {
            qCritical() << QString("%1 文件不存在！").arg(fileList.at(i));
            return false;
        }
    }

    zip.close();
    return true;
}

bool compressEpub::compressArchive(QString dirPath, QString zipPath, QString parentName)
{
    QStringList list = getAllEpubFiles(dirPath);
    QuaZip zip(zipPath);
    if(!zip.open(QuaZip::mdCreate)){
        qCritical() << QString("%1 创建失败！").arg(zipPath);
        return false;
    }
    QuaZipFile zipFile(&zip);
    for(int i = 0; i < list.size(); i++) {
        QFileInfo info(list.at(i));
        if(info.exists()) {
            QString filePath = info.filePath();
            QString fileName = filePath;
            if(parentName.isEmpty())
                fileName = fileName.replace(dirPath + "/", "");
            else
                fileName = fileName.replace(dirPath + "/", parentName + "/");
            QFile file(filePath);
            if(!file.open(QIODevice::ReadOnly)) {
                qCritical() << QString("%1 文件读取失败！").arg(filePath);
                return false;
            }
            if(!zipFile.open(QuaZipFile::WriteOnly, QuaZipNewInfo(fileName, filePath))) {
                qCritical() << QString("%1 文件写入失败！").arg(filePath);
                return false;
            }
            zipFile.write(file.readAll());
            zipFile.close();
            file.close();
        } else {
            qCritical() << QString("%1 文件不存在！").arg(list.at(i));
            return false;
        }
    }
    zip.close();
    return true;
}
