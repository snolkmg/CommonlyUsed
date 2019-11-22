#ifndef MYCOOKIEJAR_H
#define MYCOOKIEJAR_H

#include <QNetworkCookieJar>
#include <QNetworkCookie>

class myCookieJar : public QNetworkCookieJar
{
public:
    myCookieJar();

    QList<QNetworkCookie> getCookies();
    void setCookies(const QList<QNetworkCookie>& cookieList);
};

#endif // MYCOOKIEJAR_H
