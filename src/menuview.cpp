#include "menuview.hpp"
#include "config.hpp"

MenuView::MenuView(QWidget *parent)
:QListView(parent)
{
    this->setSpacing(0);
    this->setBatchSize(2000);
    this->setUniformItemSizes(true);
    this->setFrameShape(QFrame::NoFrame);
    this->setLayoutMode(QListView::Batched);
    this->setContentsMargins(0,0,0,0);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void
MenuView::setDirection(Flow flow)
{
    this->setFlow(flow);
}