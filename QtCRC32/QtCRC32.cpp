#include "QtCRC32.h"

QtCRC32::QtCRC32(QObject *parent) :
    QObject(parent)
{

}

//! 字符串 校验
QString QtCRC32::CRCString(QString text, bool isLine, bool upper)
{
#ifdef Q_OS_WIN
    text.replace("\n", "\r\n");
#endif

    return isLine ? CRCAllString(text, upper) : CRCLineString(text, upper);
}

// 字符串 校验 一次读取
QString QtCRC32::CRCAllString(QString text, bool upper)
{
    QByteArray bytes = text.toUtf8();
    const char *ch =bytes.constData();
    std::uint32_t crc = CRC::Calculate(ch, bytes.length(), CRC::CRC_32());

    QString crcText = QString("%1").arg(crc, 8, 16, QLatin1Char('0'));
    if(upper)
        crcText = crcText.toUpper();
    return crcText;
}

// 字符串 校验 多次读取
QString QtCRC32::CRCLineString(QString text, bool upper)
{
    qint64 size64 = text.length();
    qint64 constant = 8192;
    qint64 k = 0;
    qint64 hasSize = 0;

    std::uint32_t crc;

    QTextStream in(&text);
    while (!in.atEnd()) {
        QString str = in.read(constant);
        QByteArray bytes = str.toUtf8();
        const char *ch = bytes.constData();
        if(k == 0)
            crc = CRC::Calculate(ch, bytes.length(), CRC::CRC_32());
        else
            crc = CRC::Calculate(ch, bytes.length(), CRC::CRC_32(), crc);
        k++;
        hasSize = constant * k;
        if(hasSize > size64)
            hasSize = size64;
    }

    QString crcText = QString("%1").arg(crc, 8, 16, QLatin1Char('0'));
    if(upper)
        crcText = crcText.toUpper();
    return crcText;
}

//! 文件 校验
QString QtCRC32::CRCFile(QString filePath, bool isLine, bool upper)
{
    return isLine ? CRCAllFile(filePath, upper) : CRCLineFile(filePath, upper);
}

// 文件 校验 一次读取
QString QtCRC32::CRCAllFile(QString filePath, bool upper)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
        return "";

    QByteArray bytes = file.readAll();
    const char *ch = bytes.constData();
    std::uint32_t crc = CRC::Calculate(ch, bytes.length(), CRC::CRC_32());

    file.flush();
    file.close();

    QString crcText = QString("%1").arg(crc, 8, 16, QLatin1Char('0'));
    if(upper)
        crcText = crcText.toUpper();
    return crcText;
}

// 文件 校验 多次读取
QString QtCRC32::CRCLineFile(QString filePath, bool upper)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
        return "";

    qint64 size64 = file.size();
    qint64 constant = 8192;
    qint64 k = 0;
    qint64 hasSize = 0;

    std::uint32_t crc;
    while(!file.atEnd()) {
        QByteArray bytes = file.read(constant);
        const char *ch = bytes.constData();
        if(k == 0)
            crc = CRC::Calculate(ch, bytes.length(), CRC::CRC_32());
        else
            crc = CRC::Calculate(ch, bytes.length(), CRC::CRC_32(), crc);
        k++;
        hasSize = constant * k;
        if(hasSize > size64)
            hasSize = size64;
//        emit currentProgressChanged(hasSize, size64);
    }

    file.flush();
    file.close();

    QString crcText = QString("%1").arg(crc, 8, 16, QLatin1Char('0'));
    if(upper)
        crcText = crcText.toUpper();
    return crcText;
}
