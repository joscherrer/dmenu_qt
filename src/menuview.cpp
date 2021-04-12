#include "menuview.hpp"
#include "delegate.hpp"
#include "config.hpp"
#include "helper.hpp"

MenuView::MenuView(QWidget *parent)
:QListView(parent)
{
    this->setSpacing(0);
    this->setBatchSize(250);
    this->setUniformItemSizes(true);
    this->setFrameShape(QFrame::NoFrame);
    this->setLayoutMode(QListView::Batched);
    this->setContentsMargins(0,0,0,0);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setItemDelegate(new Delegate);
}

void
MenuView::setDirection(Flow flow)
{
    this->setFlow(flow);
}

