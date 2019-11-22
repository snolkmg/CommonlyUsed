#ifndef STATUSLABEL_H
#define STATUSLABEL_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QFont>
#include <QFontMetrics>

class statusLabel : public QLabel
{
    Q_OBJECT
public:
    explicit statusLabel(QWidget *parent = 0);
    explicit statusLabel(const QString &text, QWidget *parent = 0);
    void timerStart(QString msg, int msecs = 10000);

    void setLimitWidth(int limitWidth, int msecs = 10000, Qt::TextElideMode mode = Qt::ElideRight);

signals:

public slots:

private slots:
    void timerUpdate();

private:
    QTimer *timer;
};

#endif // STATUSLABEL_H
