QT       += core gui widgets

INCLUDEPATH += customWidget

SOURCES += customWidget/selectWidget.cpp \
customWidget/statusLabel.cpp \
customWidget/navigationWidget.cpp

HEADERS += customWidget/selectWidget.h\
customWidget/statusLabel.h \
customWidget/navigationWidget.h

FORMS   += customWidget/selectWidget.ui \
customWidget/navigationWidget.ui
