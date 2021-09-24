#ifndef MYLISTWIDGET_H
#define MYLISTWIDGET_H

#include <QObject>
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "NoFocusDelegate.h"

class myListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit myListWidget(QWidget *parent = 0);
    QString currentItemText();
    QPixmap currentItemPix(int w = 100, int h = 100);
    QImage currentItemImage(int w = 100, int h = 100);
    QStringList itemStringList();
    QStringList itemStringList(QRegularExpression reg, bool exclude = true);
    void setOnlySelected(QListWidgetItem *item);

Q_SIGNALS:
    void contextItemRequest(QListWidgetItem *item);

private Q_SLOTS:
    void onItemEentered(QListWidgetItem *item);

protected:
    void contextMenuEvent(QContextMenuEvent *event);
};

#endif // MYLISTWIDGET_H
