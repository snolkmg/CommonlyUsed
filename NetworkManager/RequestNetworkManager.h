#ifndef REQUESTNETWORKMANAGER_H
#define REQUESTNETWORKMANAGER_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QNetworkProxy>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QFile>
#include <QDataStream>
#include "myCookieJar.h"

class RequestNetworkManager : public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit RequestNetworkManager(QString address, QObject *parent = 0);
    explicit RequestNetworkManager(QString address, QString id, QObject *parent = 0);
    explicit RequestNetworkManager(QString address, QPair<QString, QString> tokenPair, QObject *parent = 0);
    ~RequestNetworkManager();
    void execute();
    int m_statusCode;
    QByteArray m_bytes;

    QString userAgent;

    void execPost();
    void execPost2();
    void execDelete();

    void stopWork();

signals:
    void replyJsonFinished(int statusCode, QByteArray baJson);

public slots:
    void replyFinished(QNetworkReply *reply);
    void readyRead();

private:
    QNetworkReply *m_pReply;
    QString m_address;
    QString m_id;
    QPair<QString, QString> m_tokenPair;

    myCookieJar netCookies;
};

#endif // REQUESTNETWORKMANAGER_H
