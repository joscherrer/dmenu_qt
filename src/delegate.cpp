#include <iostream>
#include "config.hpp"
#include "delegate.hpp"

Delegate::Delegate()
{
}

void Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                     const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);
}

QSize Delegate::sizeHint(const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
    if (index.data().canConvert<QString>()) {
        QFontMetrics fm(config.font);
        return QSize(fm.horizontalAdvance(index.data().toString()), fm.height());
    }
    return QStyledItemDelegate::sizeHint(option, index);
}
