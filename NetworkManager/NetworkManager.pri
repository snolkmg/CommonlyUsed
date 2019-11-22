QT       += core gui widgets network

INCLUDEPATH += NetworkManager

SOURCES += NetworkManager/RequestNetworkManager.cpp \
    NetworkManager/FileNetworkManager.cpp \
    NetworkManager/downloadRNM.cpp \
    NetworkManager/myCookieJar.cpp

HEADERS  += NetworkManager/RequestNetworkManager.h \
    NetworkManager/FileNetworkManager.h \
    NetworkManager/downloadRNM.h \
    NetworkManager/myCookieJar.h
