#include "downloadRNM.h"

downloadRNM::downloadRNM(QString fileName, QString downloadFileName, QProgressBar *pBar, int exist, QObject *parent) :
    QNetworkAccessManager(parent),
    m_fileName(fileName),
    m_downloadFileName(downloadFileName),
    m_pBar(pBar),
    m_exist(exist),
    m_hasSize(0),
    foreSize(0),
    nowSize(0),
    remainingSize(0)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &downloadRNM::speed);
    timer->start(1000);
    connect(this, SIGNAL(finished(QNetworkReply *)), this, SLOT(httpFinished(QNetworkReply *)));
}

downloadRNM::~downloadRNM()
{
    if (m_pReply != NULL)
    {
        m_pReply->abort();
        m_pReply->deleteLater();
        m_pReply = NULL;
    }
}

void downloadRNM::httpFinished(QNetworkReply *reply)
{
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    // 无错误返回
    QString msg;
    if (reply->error() == QNetworkReply::NoError) {
        file.close();
        qDebug() << reply->error() << "无错误";
        file.rename(m_downloadFileName);
        msg = QStringLiteral("下载成功");
    }
    else {
        QString strError = reply->errorString();
        qDebug() << "Error:" << strError << reply->error();
        if(m_fileSize == 0) {
            file.rename(m_downloadFileName);
            msg = QStringLiteral("下载成功");
        } else
            msg = strError;
    }
    emit replyFinished(statusCode.toInt(), m_downloadFileName, m_exist, msg);
    this->deleteLater();
}

void downloadRNM::httpReadyRead()
{
    if(m_fileSize == -1) {
        qint64 fileSize = m_pReply->header(QNetworkRequest::ContentLengthHeader).toLongLong();
        qDebug() << "文件大小：" << fileSize;
        m_fileSize = fileSize;
        emit fileSizeChanged(fileSize, m_exist);
    }
    if(m_fileSize == 0)
        file.write(0);
    else
        file.write(m_pReply->readAll());
//    qDebug() << m_downloadFileName << "Download Ready!!!";
}

void downloadRNM::updateDataReadProgress(qint64 bytesRead, qint64 totalBytes)
{
    qDebug() << bytesRead << totalBytes << "进度条";
    if(totalBytes != 0) {
        remainingSize = totalBytes - bytesRead;
        if(remainingSize == 0)
            timer->stop();
        nowSize = bytesRead;
        bytesRead += m_hasSize;
        totalBytes += m_hasSize;
        m_pBar->setRange(0, 100);
        double k = bytesRead * 1.0 / totalBytes;
        k = k * 100;
        QString str = QString::number(k, 'f', 0);
        qDebug() << str;
        m_pBar->setValue(str.toInt());
    }
}

void downloadRNM::execute()
{
    qDebug() << "真实下载地址：" << m_fileName;
    if(QFile::exists(m_downloadFileName))
        QFile::remove(m_downloadFileName);
//        QFile::remove(m_downloadFileName + ".ND");
    QNetworkRequest request;
    request.setUrl(QUrl(m_fileName));
    file.setFileName(m_downloadFileName + ".ND");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    if(QFile::exists(m_downloadFileName + ".ND")) {
        m_hasSize = file.size();
        file.seek(m_hasSize);
        QString strRange = QString("bytes=%1-").arg(m_hasSize);
        request.setRawHeader("Range", strRange.toLatin1());
    }
    qDebug() << "文件已存在大小：" << m_hasSize;
    m_pReply = get(request);
    connect(m_pReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(updateDataReadProgress(qint64,qint64)));
    connect(m_pReply, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));
}

void downloadRNM::speed()
{
    qDebug() << "计算下载速度和时间：" << foreSize << nowSize << remainingSize;
    qint64 rate = nowSize - foreSize;
    if(rate == 0)
        return;
    foreSize = nowSize;
    QString rateStr = Commons::speedStr(rate);
    int secs = remainingSize / rate;
    QString remainingTime = Commons::timeFormat(secs);
    emit speedChanged(m_exist, QString("当前速度：%1，剩余时间：%2").arg(rateStr).arg(remainingTime));
}

void downloadRNM::stopWork()
{
    timer->stop();
    if (m_pReply != NULL)
    {
        disconnect(this, SIGNAL(finished(QNetworkReply *)), this, SLOT(httpFinished(QNetworkReply *)));
        disconnect(m_pReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(updateDataReadProgress(qint64,qint64)));
        disconnect(m_pReply, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));
        m_pReply->abort();
        m_pReply->deleteLater();
        m_pReply = NULL;
    }
//    emit replyFinished(404, m_downloadFileName, m_exist, tr("已停止"));
    this->deleteLater();
}
