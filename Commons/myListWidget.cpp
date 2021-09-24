#include "myListWidget.h"

myListWidget::myListWidget(QWidget *parent) :
    QListWidget(parent)
{
    setItemDelegate(new NoFocusDelegate());
    setMouseTracking(true);
    connect(this, &myListWidget::itemEntered, this, &myListWidget::onItemEentered);
}

void myListWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QPoint point = event->pos();
    QListWidgetItem *item = itemAt(point);
    emit contextItemRequest(item);
    event->accept();
}

void myListWidget::onItemEentered(QListWidgetItem *item)
{
    if(!item || item->text().isNull())
        return;
    QToolTip::showText(QCursor::pos(), item->text(), this);
}

//当前单元格文本复制到剪贴板
QString myListWidget::currentItemText()
{
    QListWidgetItem *item = currentItem();
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

//当前单元格图标复制到剪贴板
QPixmap myListWidget::currentItemPix(int w, int h)
{
    QListWidgetItem *item = currentItem();
    if(item) {
        QIcon icon = item->icon();
        QPixmap pix = icon.pixmap(w, h);
        QClipboard *board = QApplication::clipboard();
        board->setPixmap(pix);
        return pix;
    }
    return QPixmap();
}

QImage myListWidget::currentItemImage(int w, int h)
{
    QListWidgetItem *item = currentItem();
    if(item) {
        QIcon icon = item->icon();
        QPixmap pix = icon.pixmap(w, h);
        QImage image = pix.toImage();
        QClipboard *board = QApplication::clipboard();
        board->setImage(image);
        return image;
    }
    return QImage();
}

QStringList myListWidget::itemStringList()
{
    QStringList list;
    for(int i = 0; i < this->count(); i++) {
        QString text = this->item(i)->text();
        if(!text.isEmpty())
            list << text;
    }
    return list;
}

QStringList myListWidget::itemStringList(QRegularExpression reg, bool exclude)
{
    QStringList list;
    for(int i = 0; i < this->count(); i++) {
        QString text = this->item(i)->text();
        if(exclude) {
            if(!text.contains(reg))
                list << text;
        } else {
            if(text.contains(reg))
                list << text;
        }
    }
    return list;
}

void myListWidget::setOnlySelected(QListWidgetItem *item)
{
    for(int i = 0; i < this->count(); i++)
        this->item(i)->setSelected(item == this->item(i));
}
