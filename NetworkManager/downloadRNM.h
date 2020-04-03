#ifndef DOWNLOADRNM_H
#define DOWNLOADRNM_H

#include <QNetworkAccessManager>
#include <QtNetwork>
#include <QtCore>
#include <QProgressBar>
#include "Commons.h"

class downloadRNM : public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit downloadRNM(QString fileName, QString downloadFileName, QProgressBar *pBar, int exist, QObject *parent = 0);
    ~downloadRNM();
    void execute();
    qint64 m_fileSize;

    void stopWork();

signals:
    void fileSizeChanged(qint64 fileSize, int exist);

    void replyFinished(int statusCode, QString fileName, int exist, QString msg);

    void speedChanged(int exist, QString speedStr);

public slots:

private slots:
    void httpFinished(QNetworkReply *reply);
    void httpReadyRead();
    void updateDataReadProgress(qint64 bytesRead, qint64 totalBytes);

    void speed();

private:
    QNetworkReply *m_pReply;
    QString m_fileName;
    QString m_downloadFileName;
    QProgressBar *m_pBar;
    int m_exist;
    QFile file;
    qint64 m_hasSize;

    qint64 foreSize;
    qint64 nowSize;
    qint64 remainingSize;
    QTimer *timer;
};

#endif // DOWNLOADRNM_H
