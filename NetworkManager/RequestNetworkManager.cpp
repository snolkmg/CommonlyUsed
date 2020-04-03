#include "RequestNetworkManager.h"

RequestNetworkManager::RequestNetworkManager(QString address, QObject *parent) :
    QNetworkAccessManager(parent)
{
    m_address = address;
    connect(this, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));
}

RequestNetworkManager::RequestNetworkManager(QString address, QString id, QObject *parent) :
    QNetworkAccessManager(parent)
{
    m_address = address;
    m_id = id;
    connect(this, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));
}

RequestNetworkManager::RequestNetworkManager(QString address, QPair<QString, QString> tokenPair, QObject *parent) :
    QNetworkAccessManager(parent)
{
    m_address = address;
    m_id = "";
    m_tokenPair = tokenPair;
    connect(this, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));
}

RequestNetworkManager::~RequestNetworkManager()
{
    if (m_pReply != NULL)
    {
        m_pReply->abort();
        m_pReply->deleteLater();
    }
}

void RequestNetworkManager::execute()
{
    QUrl m_url = QUrl(QString("%1").arg(m_address));
    qDebug() << "地址：" << m_address;

    QNetworkRequest request;
    QSslConfiguration conf = request.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(conf);
    request.setUrl(m_url);
    if(!userAgent.isEmpty())
        request.setHeader(QNetworkRequest::UserAgentHeader, QVariant(userAgent));

    QString header = m_tokenPair.first;
    if(!header.isEmpty()) {
        QString token = m_tokenPair.second;
        request.setRawHeader(header.toUtf8(), token.toUtf8());
    }

//    setCookieJar(&netCookies);
    m_pReply = get(request);
    connect(m_pReply, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void RequestNetworkManager::execPost()
{
    QUrl m_url = QUrl(QString("%1").arg(m_address));

    QNetworkRequest request;
    QSslConfiguration conf = request.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(conf);
    request.setUrl(m_url);
    if(!userAgent.isEmpty())
        request.setHeader(QNetworkRequest::UserAgentHeader, QVariant(userAgent));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/x-www-form-urlencoded"));

    QString header = m_tokenPair.first;
    if(!header.isEmpty()) {
        QString token = m_tokenPair.second;
        request.setRawHeader(header.toUtf8(), token.toUtf8());
    }

    QByteArray ba;
    ba.append(m_id);
    m_pReply = post(request, ba);
    connect(m_pReply, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void RequestNetworkManager::execPost2()
{
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentTypeHeader, QLatin1String("multipart/form-data;"));
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant(QString("form-data; name=\"file\"; filename=\"temp.txt\"")));

    QByteArray ba = m_id.toUtf8();
    filePart.setBody(ba);
    multiPart->append(filePart);

    QUrl m_url = QUrl(QString("%1").arg(m_address));

    QNetworkRequest request;
    QSslConfiguration conf = request.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(conf);
    request.setUrl(m_url);
    request.setHeader(QNetworkRequest::UserAgentHeader, QVariant(userAgent));
    m_pReply = post(request, multiPart);
    connect(m_pReply, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void RequestNetworkManager::execDelete()
{
    QUrl m_url = QUrl(QString("%1").arg(m_address));

    QNetworkRequest request;
    QSslConfiguration conf = request.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(conf);
    request.setUrl(m_url);
    request.setHeader(QNetworkRequest::UserAgentHeader, QVariant(userAgent));
    m_pReply = deleteResource(request);
    connect(m_pReply, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void RequestNetworkManager::replyFinished(QNetworkReply *reply)
{
//    qDebug() << "X-User-Token:" << reply->rawHeader(m_tokenPair.first.toUtf8());
    // 获取响应的信息，200表示正常
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    QByteArray bytes;

    if (reply->error() == QNetworkReply::NoError) {
        bytes = reply->readAll();
    }
    else {
        QString strError = reply->errorString();
        qDebug() << "Error:" << strError;
    }

    m_statusCode = statusCode.toInt();
    m_bytes = bytes;

//    QList<QNetworkCookie> cookies = netCookies.getCookies();

//    qDebug() << "cookies:" << cookies;

    emit replyJsonFinished(m_statusCode, m_bytes);
    deleteLater();
}

// 本地写文件
void RequestNetworkManager::readyRead()
{

}

void RequestNetworkManager::stopWork()
{
    if (m_pReply != NULL)
    {
        disconnect(this, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));
        disconnect(m_pReply, SIGNAL(readyRead()), this, SLOT(readyRead()));
        m_pReply->abort();
        m_pReply->deleteLater();
        m_pReply = NULL;
    }
    this->deleteLater();
}

