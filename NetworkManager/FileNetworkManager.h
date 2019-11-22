#ifndef FILENETWORKMANAGER_H
#define FILENETWORKMANAGER_H

#include <QtCore>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QProgressBar>
#include "commons.h"

class FileNetworkManager : public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit FileNetworkManager(QString address, QPair<QString, QString> tokenPair, QObject *parent = 0);
    explicit FileNetworkManager(QStringList fileList, QString address, QObject *parent = 0, bool isUpload = false);
    explicit FileNetworkManager(QByteArray byteArray, QString address, QObject *parent = 0);
    explicit FileNetworkManager(QString address, QString fileName, QString filePath, QPair<QString, QString> tokenPair,
                                QString belongId, QString fileType, QObject *parent = 0);
    ~FileNetworkManager();
    void execute();
    void upload();
    void change();
    void add74Book();

    void deleteExeute();

    QStringList m_paraList;
    qint64 m_hasSize;
    int m_exist;
    QProgressBar *m_pBar;

    int m_statusCode;
    QByteArray m_bytes;

signals:
    void replyFileFinished(int statusCode, QByteArray baJson);
    void sendRate(QString rate);

    void speedChanged(int exist, QString speedStr);

private slots:
    void replyFinished(QNetworkReply *reply);
    void readyRead();
    void onUploadProgress(qint64 bytesSent, qint64 bytesTotal);

    void speed();

private:
    QNetworkReply *m_pReply;
    QStringList m_fileList;
    QByteArray m_byteArray;
    QString m_address;
    QString m_fileName;
    QString m_filePath;
    bool m_isUpload;

    QPair<QString, QString> m_tokenPair;
    QString m_belongId;
    QString m_fileType;

    qint64 foreSize;
    qint64 nowSize;
    qint64 remainingSize;
    QTimer *timer;

    QNetworkRequest requestSetRawHeader(QNetworkRequest request);

    QString speedMsg();
};

#endif // FILENETWORKMANAGER_H
