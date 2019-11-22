#include "myComboBox.h"

myComboBox::myComboBox(QWidget *parent) :
    QComboBox(parent)
{

}

QStringList myComboBox::textList()
{
    QStringList list;
    for(int i = 0; i < count(); i++)
        list << itemText(i);
    return list;
}

void myComboBox::removeAll()
{
    int count = this->count();
    for(int i = 0; i < count; i++)
        removeItem(0);
}

void myComboBox::modelSetting(QStringList list)
{
    QStandardItemModel *model = new QStandardItemModel();
    for(int i = 0 ; i < list.size(); i++) {
        QStandardItem *item = new QStandardItem(list.at(i));;
        item->setToolTip(list.at(i));
        model->appendRow(item);
    }
    this->setModel(model);
}
