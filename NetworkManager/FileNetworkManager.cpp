#include "FileNetworkManager.h"

qint64 MAX = 5;

FileNetworkManager::FileNetworkManager(QString address, QPair<QString, QString> tokenPair, QObject *parent) :
    QNetworkAccessManager(parent),
    m_address(address),
    m_tokenPair(tokenPair)
{
    connect(this, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));
}

FileNetworkManager::FileNetworkManager(QStringList fileList, QString address, QObject *parent, bool isUpload) :
    QNetworkAccessManager(parent),
    m_fileList(fileList),
    m_address(address),
    m_isUpload(isUpload),
    foreSize(0),
    nowSize(0),
    remainingSize(0)
{
    if(m_isUpload) {
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &FileNetworkManager::speed);
        timer->start(1000);
    }
    connect(this, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));
}

FileNetworkManager::FileNetworkManager(QByteArray byteArray, QString address, QObject *parent) :
    QNetworkAccessManager(parent),
    m_byteArray(byteArray),
    m_address(address)
{
    connect(this, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));
}

FileNetworkManager::FileNetworkManager(QString address, QString fileName, QString filePath, QPair<QString, QString> tokenPair,
                                       QString belongId, QString fileType, QObject *parent) :
    QNetworkAccessManager(parent)
{
    m_address = address;
    m_fileName = fileName;
    m_filePath = filePath;
    m_isUpload = true;
    m_tokenPair = tokenPair;
    m_belongId = belongId;
    m_fileType = fileType;
    foreSize = 0;
    nowSize = 0;
    remainingSize = 0;

    if(m_isUpload) {
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &FileNetworkManager::speed);
        timer->start(1000);
    }
    connect(this, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));
}

FileNetworkManager::~FileNetworkManager()
{
    if (m_pReply != NULL)
    {
        m_pReply->abort();
        m_pReply->deleteLater();
        m_pReply = NULL;
    }
}

QNetworkRequest FileNetworkManager::requestSetRawHeader(QNetworkRequest request)
{
    QString header = m_tokenPair.first;
    if(!header.isEmpty()) {
        QString token = m_tokenPair.second;
        request.setRawHeader(header.toUtf8(), token.toUtf8());
    }
    return request;
}

void FileNetworkManager::upload()
{
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentTypeHeader, QLatin1String("application/zip;"));
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant(QString("form-data; name=\"file\"; filename=\"%1\"").arg(m_fileName)));

    QFile *file = new QFile(m_filePath);
    file->open(QIODevice::ReadOnly);
    filePart.setBodyDevice(file);
    file->setParent(multiPart);
    multiPart->append(filePart);

    QNetworkRequest request;
    QUrl m_url = QUrl(QString(m_address));

    QUrlQuery query;
    if(!m_belongId.isEmpty())
        query.addQueryItem("belongId", m_belongId);
    if(!m_fileType.isEmpty())
        query.addQueryItem("fileType", m_fileType);

    m_url.setQuery(query);

    request = requestSetRawHeader(request);

    qDebug() << "服务器地址：" << m_url;

    request.setUrl(m_url);

    m_pReply = post(request, multiPart);
    multiPart->setParent(m_pReply);
    connect(m_pReply, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(m_pReply, SIGNAL(uploadProgress(qint64,qint64)), this, SLOT(onUploadProgress(qint64,qint64)));
    connect(m_pReply, &QNetworkReply::finished, [this]{
        if(m_pReply != NULL) {
            m_pReply->deleteLater();
            m_pReply = 0;
        }
    });
}

void FileNetworkManager::appendFile()
{
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentTypeHeader, QLatin1String("multipart/form-data;"));
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant(QString("form-data; name=\"file\"; filename=\"%1\"").arg(m_fileName)));

    QFile *file = new QFile(m_filePath);
    file->open(QIODevice::ReadOnly);
    filePart.setBodyDevice(file);
    file->setParent(multiPart);
    multiPart->append(filePart);

    QNetworkRequest request;
    QUrl m_url = QUrl(QString(m_address));

    request = requestSetRawHeader(request);

    qDebug() << "服务器地址：" << m_url;

    request.setUrl(m_url);

    m_pReply = put(request, multiPart);
    multiPart->setParent(m_pReply);
    connect(m_pReply, SIGNAL(readyRead()), this, SLOT(readyRead()));
//    connect(m_pReply, SIGNAL(uploadProgress(qint64,qint64)), this, SLOT(onUploadProgress(qint64,qint64)));
    connect(m_pReply, &QNetworkReply::finished, [this]{
        if(m_pReply != NULL) {
            m_pReply->deleteLater();
            m_pReply = 0;
        }
    });
}

void FileNetworkManager::deleteExeute()
{
    qDebug() << "删除地址：" << m_address;
    QUrl m_url = QUrl(QString(m_address));
    QNetworkRequest request;
    request = requestSetRawHeader(request);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QLatin1String("application/x-www-form-urlencoded;"));
    request.setUrl(m_url);

    m_pReply = deleteResource(request);
    connect(m_pReply, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void FileNetworkManager::execute()
{
    qDebug() << "post地址：" << m_address;
    QUrl m_url = QUrl(QString(m_address));
    QNetworkRequest request;
    request = requestSetRawHeader(request);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QLatin1String("application/x-www-form-urlencoded;"));

    request.setUrl(m_url);
    m_pReply = post(request, QByteArray());
    connect(m_pReply, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void FileNetworkManager::change()
{
    qDebug() << "put地址：" << m_address;
    QUrl m_url = QUrl(QString(m_address));
    QNetworkRequest request;
    request = requestSetRawHeader(request);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QLatin1String("application/x-www-form-urlencoded;"));
    request.setUrl(m_url);
    m_pReply = put(request, QByteArray(""));
    connect(m_pReply, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void FileNetworkManager::add74Book()
{
    QUrl m_url = QUrl(QString(m_address));
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, QLatin1String("application/json;"));
    request.setUrl(m_url);
    QByteArray ba;
    ba.append("bookInfoParams=").append(m_byteArray);
    m_pReply = post(request, m_byteArray);
    connect(m_pReply, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void FileNetworkManager::replyFinished(QNetworkReply *reply)
{
    // 获取响应的信息，状态码为200表示正常
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    qDebug() << "状态码：" << statusCode;

    QByteArray bytes;
    // 无错误返回
    if (reply->error() == QNetworkReply::NoError) {
        bytes = reply->readAll();  //获取字节
    }
    else {
        QString strError = reply->errorString();
        qDebug() << "Error:" << strError << reply->error();
    }

    qDebug() << "字节数：" << QString(bytes);
    m_statusCode = statusCode.toInt();
    m_bytes = bytes;
    emit replyFileFinished(m_statusCode, m_bytes);
    emit speedChanged(m_exist, QString("上传成功"));
    this->deleteLater();
}

// 本地写文件
void FileNetworkManager::readyRead()
{
    qDebug() << "File Ready!!!";
}

void FileNetworkManager::onUploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    if(bytesTotal != 0) {
        if(m_isUpload) {
            remainingSize = bytesTotal - bytesSent;
            if(remainingSize == 0) {
                timer->stop();
                emit speedChanged(m_exist, QString("服务器正在处理文件，请稍后……"));
            }
        }
        nowSize = bytesSent;
        qDebug() << bytesSent << bytesTotal;
        m_pBar->setRange(0, 100);
        double k = bytesSent * 1.0 / bytesTotal;
        k = k * 100;
        QString str = QString::number(k, 'f', 0);
        qDebug() << str;
        m_pBar->setValue(str.toInt());
        QString rate = str == "100" ? QString("服务器正在处理文件，请稍后……")
                                    : QString("正在上传文件 %1%").arg(str);
        emit sendRate(rate);
    }
}

void FileNetworkManager::speed()
{
    qDebug() << "计算上传速度和时间：" << foreSize << nowSize << remainingSize;
    qint64 rate = nowSize - foreSize;
    if(rate == 0)
        return;
    foreSize = nowSize;
    QString rateStr = Commons::speedStr(rate);
    int secs = remainingSize / rate;
    QString remainingTime = Commons::timeFormat(secs);
    QString str = QString("当前速度：%1，剩余时间：%2").arg(rateStr).arg(remainingTime);
    qDebug() << str;
    emit speedChanged(m_exist, str);
}

void FileNetworkManager::uploadBuffer()
{
    timer->stop();
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentTypeHeader, QLatin1String("application/zip;"));
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant(QString("form-data; name=\"file\"; filename=\"%1\"").arg(m_fileName)));

//    QFile *file = new QFile(m_buffer);
//    file->open(QIODevice::ReadOnly);
//    filePart.setBodyDevice(file);
//    file->setParent(multiPart);
//    multiPart->append(filePart);

    m_buffer->open(QIODevice::ReadOnly);
    filePart.setBodyDevice(m_buffer);
    m_buffer->setParent(multiPart);
    multiPart->append(filePart);

    QNetworkRequest request;
    QUrl m_url = QUrl(QString(m_address));

    QUrlQuery query;
    if(!m_belongId.isEmpty())
        query.addQueryItem("belongId", m_belongId);
    if(!m_fileType.isEmpty())
        query.addQueryItem("fileType", m_fileType);

    m_url.setQuery(query);

    request = requestSetRawHeader(request);

    qDebug() << "服务器地址：" << m_url;

    request.setUrl(m_url);

    m_pReply = post(request, multiPart);
    multiPart->setParent(m_pReply);
    connect(m_pReply, SIGNAL(readyRead()), this, SLOT(readyRead()));
//    connect(m_pReply, SIGNAL(uploadProgress(qint64,qint64)), this, SLOT(onUploadProgress(qint64,qint64)));
    connect(m_pReply, &QNetworkReply::finished, [this]{
        if(m_pReply != NULL) {
            m_pReply->deleteLater();
            m_pReply = 0;
        }
    });
}

void FileNetworkManager::stopWork()
{
    if(timer)
        timer->stop();
    if (m_pReply != NULL)
    {
        disconnect(this, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));
        disconnect(m_pReply, SIGNAL(uploadProgress(qint64,qint64)), this, SLOT(onUploadProgress(qint64,qint64)));
        disconnect(m_pReply, SIGNAL(readyRead()), this, SLOT(readyRead()));
        m_pReply->abort();
        m_pReply->deleteLater();
        m_pReply = NULL;
    }
    this->deleteLater();
}

