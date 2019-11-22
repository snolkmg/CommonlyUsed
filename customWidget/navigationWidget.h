#ifndef NAVIGATIONWIDGET_H
#define NAVIGATIONWIDGET_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

namespace Ui {
class navigationWidget;
}

class navigationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit navigationWidget(QWidget *parent = 0);
    ~navigationWidget();
    void searchNull();
    void setNavWidget(QJsonObject dataObj);
    void setShowSize(int value);
    void setButtonFixedSize(int size);

    void setNavWidget2(QJsonObject dataObj);

    int pageNum();

public Q_SLOTS:
    int pageSize();

signals:
    void searchChanged(QString str);
    void pageChanged(int value);

private Q_SLOTS:
    void onValueChanged(int value);

    void timerUpdate();

private:
    Ui::navigationWidget *ui;
    void initWidget();
    QTimer *mTimer;
};

#endif // NAVIGATIONWIDGET_H
