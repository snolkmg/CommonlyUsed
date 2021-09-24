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
//    connect(vScrollBar, &QScrollBar::valueChanged, this, &myTableWidget::onScrollValueChanged);
}

void myTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QPoint point = event->pos();
//    qDebug() << "myTableWidget：" << point;
    QTableWidgetItem *item = itemAt(point);
    emit itemSender(item);
    event->accept();
}

void myTableWidget::onScrollValueChanged(int value)
{
    Q_UNUSED(value)
//    qDebug() << "滚动条：" << vScrollBar->minimum() << vScrollBar->maximum() << value;
//    if(vScrollBar->maximum() - value < 10)
//        scrollToBottom();
}

//设置单元格文本，是否居中
QTableWidgetItem *myTableWidget::itemSetting(int row, int col, QString text, bool center)
{
    QTableWidgetItem *item = new QTableWidgetItem(text);
    if(center)
        item->setTextAlignment(Qt::AlignCenter);
    setItem(row, col, item);
    return item;
}

//删除所有列
void myTableWidget::removeAllItems()
{
    int count = this->rowCount();
    for(int i = 0; i < count; i++)
        this->removeRow(0);
}

//获取第col列选中的item列表
QList<QTableWidgetItem *> myTableWidget::checkedItems(int col)
{
    QList<QTableWidgetItem *> items;
    for(int i = 0; i < rowCount(); i++) {
        if(this->item(i, col)->checkState() == Qt::Checked)
            items.append(this->item(i, col));
    }
    return items;
}

//根据第col列是否选中，来获取第id列的文本列表
QStringList myTableWidget::checkedItemsList(int id, int col)
{
    QStringList list;
    for(int i = 0; i < rowCount(); i++) {
        if(this->item(i, col)->checkState() == Qt::Checked)
            list.append(this->item(i, id)->text());
    }
    return list;
}

//根据特定一列spe的文本是否等于arg，来获取与其同一行的第col列的文本列表
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

//第column列统计文本为arg的单元格数量
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

//第column列查找文本为arg的单元格
QTableWidgetItem * myTableWidget::findItemByArg(int column, QString arg)
{
    QTableWidgetItem *item;
    for(int i = 0; i < this->rowCount(); i++) {
        QString text = this->item(i, column)->text();
        if(arg == text) {
            item = this->item(i, 0);
            return item;
        }
    }
    return nullptr;
}

//统计第column列checked数量
int myTableWidget::countChecked(int column)
{
    int k = 0;
    for(int i = 0; i < this->rowCount(); i++) {
        if(this->item(i, column)->checkState() == Qt::Checked)
            k++;
    }
    return k;
}

//判断第column列文本是否包含arg字符
bool myTableWidget::containByCol(int column, QString arg)
{
    for(int i = 0; i < rowCount(); i++) {
        if(this->item(i, column)->text() == arg)
            return true;
    }
    return false;
}

//为所有单元格文本设置统一值（通常用于初始化）
void myTableWidget::setAllItemText(QString arg)
{
    for(int i = 0; i < rowCount(); i++) {
        for(int j = 0; j < columnCount(); j++)
            itemSetting(i, j, arg);
    }
}

//当前单元格文本复制到剪贴板
QString myTableWidget::currentItemText()
{
    QTableWidgetItem *item = currentItem();
    if(item) {
        QString text = item->text();
        if(text.isEmpty())
            return "";
        QClipboard *board = QApplication::clipboard();
        board->setText(text);
        return text;
    }
    return "";
}

//当前行指定列单元格文本复制到剪贴板
QString myTableWidget::columnItemText(int column)
{
    QTableWidgetItem *cItem = currentItem();
    QTableWidgetItem *item = this->item(cItem->row(), column);
    if(item) {
        QString text = item->text();
        if(text.isEmpty())
            return "";
        QClipboard *board = QApplication::clipboard();
        board->setText(text);
        return text;
    }
    return "";
}
