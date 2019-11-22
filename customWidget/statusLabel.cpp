#include "statusLabel.h"

statusLabel::statusLabel(QWidget *parent) :
    QLabel(parent)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &statusLabel::timerUpdate);
}

statusLabel::statusLabel(const QString &text, QWidget *parent) :
    QLabel(parent)
{
    setText(text);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &statusLabel::timerUpdate);
}

void statusLabel::timerUpdate()
{
    clear();
    toolTip().clear();
}

void statusLabel::timerStart(QString msg, int msecs)
{
    setText(msg);
//    setToolTip(msg);
    if(timer->isActive())
        timer->stop();
    if(msecs > 0)
        timer->start(msecs);
}

void statusLabel::setLimitWidth(int limitWidth, int msecs, Qt::TextElideMode mode)
{
    QFontMetrics fontMetrics(this->font());
    int fontSize = fontMetrics.width(this->text());
    QString str = this->text();
    if(fontSize > limitWidth)
        str = fontMetrics.elidedText(this->text(), mode, limitWidth);
    this->timerStart(str, msecs);
}
