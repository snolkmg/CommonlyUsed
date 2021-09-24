#ifndef NOFOCUSDELEGATE_H
#define NOFOCUSDELEGATE_H

#include <QPainter>
#include <QStyle>
#include <QStyledItemDelegate>
#include <QStyleOptionViewItem>
#include <QModelIndex>

class NoFocusDelegate : public QStyledItemDelegate
{
public:
    virtual void paint(QPainter * painter, const QStyleOptionViewItem & option,
                       const QModelIndex & index) const
    {
        QStyleOptionViewItem itemOption(option);
        if (itemOption.state & QStyle::State_HasFocus) {
            itemOption.state = itemOption.state ^ QStyle::State_HasFocus;
        }
        QStyledItemDelegate::paint(painter, itemOption, index);
    }
};

#endif // NOFOCUSDELEGATE_H
