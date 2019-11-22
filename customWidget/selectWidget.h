#ifndef SELECTWIDGET_H
#define SELECTWIDGET_H

#include <QWidget>
#include <QtCore>
#include <QtGui>
#include <QtWidgets>

namespace Ui {
class selectWidget;
}

class selectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit selectWidget(QWidget *parent = 0);
    ~selectWidget();
    void setTableWidget(QTableWidget *tableWidget);
    void setUnCheck();

signals:
    void checkedNumber(int count);

private:
    Ui::selectWidget *ui;
    QTableWidget *m_tableWidget;

    void clearAllChecked();
    void selectAllChecked();
    void reverseAllChecked();
};

#endif // SELECTWIDGET_H
