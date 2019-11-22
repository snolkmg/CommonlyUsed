#include "myCookieJar.h"

myCookieJar::myCookieJar()
{

}

QList<QNetworkCookie> myCookieJar::getCookies()
{
    return allCookies();
}

void myCookieJar::setCookies(const QList<QNetworkCookie>& cookieList)
{
    if(this == nullptr) {
        return;
    }
    setAllCookies(cookieList);
}
