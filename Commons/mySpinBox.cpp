#include "mySpinBox.h"

mySpinBox::mySpinBox(QWidget *parent) :
    QSpinBox(parent)
{
    setTimer();
    connect(this, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &mySpinBox::onValueChanged);
}

void mySpinBox::setTimer()
{
    timer = new QTimer;
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, this, &mySpinBox::onTimeout);
}

void mySpinBox::onValueChanged(int value)
{
    emit valueIsChanging(value);
    timer->start(1000);
}

void mySpinBox::onTimeout()
{
    emit valueHasChanged();
}
