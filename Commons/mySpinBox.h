#ifndef MYSPINBOX_H
#define MYSPINBOX_H

#include <QObject>
#include <QWidget>
#include <QSpinBox>
#include <QTimer>

class mySpinBox : public QSpinBox
{
    Q_OBJECT
public:
    explicit mySpinBox(QWidget *parent = 0);

Q_SIGNALS:
    void valueIsChanging(int value);
    void valueHasChanged();

private Q_SLOTS:
    void onTimeout();
    void onValueChanged(int value);

private:
    QTimer *timer;
    void setTimer();
};

#endif // MYSPINBOX_H
