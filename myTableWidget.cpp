#include "myTableWidget.h"

myTableWidget::myTableWidget(QWidget *parent) :
    QTableWidget(parent)
{
//    cHeaderView = new CheckBoxHeaderView(0, Qt::Horizontal, this);
//    setHorizontalHeader(cHeaderView);
    horizontalHeader()->setDefaultSectionSize(100);
    horizontalHeader()->setSectionsClickable(false); //设置表头不可点击（默认点击后进行排序）

    setItemDelegate(new NoFocusDelegate());
    horizontalHeader()->setHighlightSections(false);

    horizontalHeader()->setStretchLastSection(true);
    verticalHeader()->setDefaultSectionSize(30);

//    horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
//    verticalHeader()->setDefaultSectionSize(50);
//    verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //    verticalHeader()->setDefaultSectionSize(10); //设置行高
    //    setFrameShape(QFrame::NoFrame); //设置无边框
    verticalHeader()->setVisible(false); //设置垂直头不可见
    setSelectionMode(QAbstractItemView::ExtendedSelection);  //可多选（Ctrl、Shift、  Ctrl+A都可以）
    setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行
    setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    //    horizontalHeader()->setFixedHeight(25); //设置表头的高度
    setMouseTracking(true);
    setAlternatingRowColors(true);

    horizontalHeader()->setStyleSheet(QString("font-weight: bold;"));
    verticalHeader()->setStyleSheet(QString("font-weight: bold;"));

//    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(this, &QTableWidget::cellEntered, [this](int row, int column) {
        if(item(row, column) == NULL || item(row, column)->text().isNull())
            return;
        else
            QToolTip::showText(QCursor::pos(), item(row, column)->text(), this);
    });

//    connect(cHeaderView, &CheckBoxHeaderView::checkStausChange, [this](bool check){
//        qDebug() << "表头已选：" << check;
//    });
//    connect(this, &myTableWidget::isTristate, cHeaderView, &CheckBoxHeaderView::getTristate);

    vScrollBar = this->verticalScrollBar();
    connect(vScrollBar, &QScrollBar::valueChanged, this, &myTableWidget::onValueChanged);
}

void myTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QPoint point = event->pos();
    qDebug() << "myTableWidget：" << point;
    QTableWidgetItem *item = itemAt(point);
    emit itemSender(item);
    event->accept();
}

void myTableWidget::onValueChanged(int value)
{
    qDebug() << "滚动条：" << vScrollBar->minimum() << vScrollBar->maximum() << value;
//    if(vScrollBar->maximum() - value < 10)
//        scrollToBottom();
}

QTableWidgetItem *myTableWidget::itemSetting(int row, int col, QString text, bool center)
{
    QTableWidgetItem *item = new QTableWidgetItem(text);
    if(center)
        item->setTextAlignment(Qt::AlignCenter);
    setItem(row, col, item);
    return item;
}

void myTableWidget::removeAllItems()
{
    int count = this->rowCount();
    for(int i = 0; i < count; i++)
        this->removeRow(0);
}

QList<QTableWidgetItem *> myTableWidget::checkedItems(int col)
{
    QList<QTableWidgetItem *> items;
    for(int i = 0; i < rowCount(); i++) {
        if(this->item(i, col)->checkState() == Qt::Checked)
            items.append(this->item(i, col));
    }
    return items;
}

QStringList myTableWidget::checkedItemsList(int id, int col)
{
    QStringList list;
    for(int i = 0; i < rowCount(); i++) {
        if(this->item(i, col)->checkState() == Qt::Checked)
            list.append(this->item(i, id)->text());
    }
    return list;
}

QStringList myTableWidget::speicalList(QString arg, int spe, bool ok, int col)
{
    QStringList list;
    for(int i = 0; i < rowCount(); i++) {
        if(ok) {
            if(this->item(i, spe)->text() == arg)
                list.append(this->item(i, col)->text());
        } else {
            if(this->item(i, spe)->text() != arg)
                list.append(this->item(i, col)->text());
        }
    }
    return list;
}

int myTableWidget::countByArg(int column, QString arg)
{
    int k = 0;
    for(int i = 0; i < this->columnCount(); i++) {
        QString text = this->item(i, column)->text();
        if(text == arg)
            k++;
    }
    return k;
}

QTableWidgetItem * myTableWidget::findItemByArg(int column, QString arg)
{
    QTableWidgetItem *item;
    for(int i = 0; i < this->rowCount(); i++) {
        QString text = this->item(i, column)->text();
        if(arg == text) {
            item = this->item(i, 0);
            break;
        }
    }
    return item;
}

int myTableWidget::countChecked(int column)
{
    int k = 0;
    for(int i = 0; i < this->rowCount(); i++) {
        if(this->item(i, column)->checkState() == Qt::Checked)
            k++;
    }
    return k;
}
