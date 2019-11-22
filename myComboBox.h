#ifndef MYCOMBOBOX_H
#define MYCOMBOBOX_H

#include <QObject>
#include <QWidget>
#include <QComboBox>
#include <QStandardItemModel>

class myComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit myComboBox(QWidget *parent = 0);
    QStringList textList();
    void removeAll();
    void modelSetting(QStringList list);

};

#endif // MYCOMBOBOX_H
