#ifndef MYTABLEWIDGET_H
#define MYTABLEWIDGET_H

#include <QObject>
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "NoFocusDelegate.h"

class myTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit myTableWidget(QWidget *parent = 0);
    QTableWidgetItem * itemSetting(int row, int col, QString text, bool center = true);
    void removeAllItems();
    QList<QTableWidgetItem *> checkedItems(int col = 0);
    QStringList checkedItemsList(int id, int col = 0);
    QStringList speicalList(QString arg, int spe, bool ok = true, int col = 0);

    int countByArg(int column, QString arg);

    QTableWidgetItem *findItemByArg(int column, QString arg);

    int countChecked(int column = 0);

signals:
    void itemSender(QTableWidgetItem *item);
    void isTristate(bool tristate);
    void onItemSelectionChanged(Qt::CheckState state);

private slots:
    void onValueChanged(int value);

protected:
    void contextMenuEvent(QContextMenuEvent *event);

private:
//    CheckBoxHeaderView *cHeaderView;
    QScrollBar *vScrollBar;
};

#endif // MYTABLEWIDGET_H
