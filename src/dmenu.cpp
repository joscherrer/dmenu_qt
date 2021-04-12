#include "dmenu.hpp"
#include "textbox.hpp"
#include "stdin.hpp"
#include "config.hpp"
#include "helper.hpp"
#include "menuview.hpp"
#include "delegate.hpp"

#include <iostream>
#include <QFile>
#include <QFontDatabase>
#include <QVBoxLayout>
#include <QRegularExpression>


Dmenu::Dmenu(QWidget *parent)
:QFrame(parent, Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint)
{
    h.timestamp("Dmenu init start");
    this->textBox = new TextBox(this);
    this->menuView = new MenuView(this);
    this->menuModel = new QStringListModel(this);
    this->menuProxyModel = new QSortFilterProxyModel(this);

    this->menuProxyModel->setSourceModel(this->menuModel);
    this->menuProxyModel->setFilterCaseSensitivity(static_cast<Qt::CaseSensitivity>(config.sensitive));

    this->menuView->setModel(this->menuProxyModel);

    /* 
        Ugly hack to prevent :hover styling. A better approach would be implementing
        a custom QStyle or overriding paintEvent on a custom QListView 
    */
    this->menuView->setStyleSheet("QListWidget::item:hover,QListWidget::item:disabled:hover,QListWidget::item:hover:!active,{background: transparent;}");

    auto mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(QMargins(0,0,0,0));
    mainLayout->setSpacing(0);
    mainLayout->addWidget(this->textBox);
    mainLayout->addWidget(this->menuView);
    
    this->setLayout(mainLayout);
    this->setLineWidth(config.border_width);
    this->setFrameStyle(QFrame::Box | QFrame::Plain);

    QObject::connect(this->textBox, &QLineEdit::textChanged, this, [this](){
        this->menuProxyModel->setFilterWildcard(this->textBox->text());
        this->selectRow(0);
    });
    h.timestamp("Dmenu init end");
    // this->startThread();
}

bool
Dmenu::event(QEvent *e)
{
    if (e->type() == QEvent::KeyPress)
    {
        QKeyEvent *ke = static_cast<QKeyEvent *>(e);
        if (ke->key() == Qt::Key_Escape)
        {
            this->close();
            return true;
        }
        else if (ke->key() == Qt::Key_Return)
        {
            if (this->menuProxyModel->rowCount() == 0)
                std::cout << this->textBox->text().toStdString() << std::endl;
            else
                std::cout << this->menuView->currentIndex().data(Qt::DisplayRole).toString().toStdString() << std::endl;
            this->close();
            return true;
        }
        else if (ke->key() == Qt::Key_Down)
        {
            QModelIndex index = this->menuView->currentIndex();
            QModelIndex newIndex = this->selectRow(index.row() + 1);
            if ((index.row() + 1) % config.lines == 0 && index.row() + 1 != 0)
                this->menuView->scrollTo(newIndex, QAbstractItemView::PositionAtTop);
            return true;
        }
        else if (ke->key() == Qt::Key_Up)
        {
            QModelIndex index = this->menuView->currentIndex();
            QModelIndex newIndex = this->selectRow(index.row() - 1);
            if (index.row() % config.lines == 0 && newIndex.isValid())
                this->menuView->scrollTo(newIndex, QAbstractItemView::PositionAtBottom);
            return true;
        }
        else if (ke->key() == Qt::Key_Left)
        {
        }
        else if (ke->key() == Qt::Key_Right)
        {
        }
        else if (ke->key() == Qt::Key_PageDown)
        {
            QModelIndex index = this->menuView->currentIndex();
            QModelIndex newIndex = this->selectRow((index.row() / config.lines + 1) * config.lines);
            this->selectRow(newIndex.row());
            this->menuView->scrollTo(newIndex, QAbstractItemView::PositionAtTop);
            return true;
        }
        else if (ke->key() == Qt::Key_PageUp)
        {
            QModelIndex index = this->menuView->currentIndex();
            QModelIndex newIndex = this->selectRow((index.row() / config.lines - 1) * config.lines);
            this->selectRow(newIndex.row());
            this->menuView->scrollTo(newIndex, QAbstractItemView::PositionAtTop);
            return true;
        }
    }
    else if (e->type() == QEvent::ShortcutOverride)
    {
        QKeyEvent *ke = static_cast<QKeyEvent *>(e);
        if (ke->key() == Qt::Key_Tab)
        {
            this->textBox->setText(this->menuView->currentIndex().data(Qt::DisplayRole).toString());
            this->textBox->setFocus(Qt::OtherFocusReason);
            this->textBox->grabKeyboard();
            e->accept();
            return true;
        }
    }
    else if (e->type() == QEvent::Polish)
    {
        h.timestamp("Window visible");
    }
    return QFrame::event(e);
}

QModelIndex
Dmenu::selectRow(int row)
{
    QModelIndex index = this->menuProxyModel->index(row, 0);
    if (index.isValid())
        this->menuView->setCurrentIndex(index);
    return index;
}

void
Dmenu::focusOutEvent(QFocusEvent *e)
{
    e->accept();
    this->setFocus();
}

void
Dmenu::addEntry(QStringList &entry)
{
    this->menuModel->setStringList(entry);
}

void
Dmenu::set_data()
{
    h.timestamp("Start set model");
    this->menuModel->setStringList(this->sr->data);
    h.timestamp("End set model");
}

void
Dmenu::fitToContent()
{
    QFontMetrics fm(config.font);
    QSize qs(600, config.lines * fm.height() + this->textBox->height() + config.border_width * 2);
    this->resize(qs);
    this->setFixedSize(qs);
}