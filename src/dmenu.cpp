#include "dmenu.hpp"
#include "textbox.hpp"
#include "stdin.hpp"
#include "config.hpp"
#include "helper.hpp"
#include <iostream>
#include <QFile>
#include <QFontDatabase>
#include <QVBoxLayout>
#include <QRegularExpression>



Dmenu::Dmenu(bool sensitive, QWidget *parent)
:QFrame(parent)
{
    this->textBox = new TextBox(this);
    this->menuModel = new QStringListModel(this);
    this->menuView = new QListView(this);
    this->menuProxyModel = new QSortFilterProxyModel(this);

    // this->menuView->setStyleSheet("QScrollBar { width:0px; } QListView::item::selected { background-color: #edccea; color: #484848; }");

    this->menuProxyModel->setSourceModel(this->menuModel);
    this->menuProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    this->menuView->setModel(this->menuProxyModel);
    this->menuView->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->menuView->setUniformItemSizes(true);
    this->menuView->setLayoutMode(QListView::Batched);
    this->menuView->setBatchSize(250);
    this->menuView->setContentsMargins(QMargins(0,0,0,0));
    this->menuView->setFrameShape(QFrame::NoFrame);

    this->menuView->setStyleSheet("QListWidget::item:hover,QListWidget::item:disabled:hover,QListWidget::item:hover:!active,{background: transparent;}");
    this->textBox->setFocusPolicy(Qt::TabFocus);

    auto mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(QMargins(0,0,0,0));
    mainLayout->setSpacing(0);
    mainLayout->addWidget(this->textBox);
    mainLayout->addWidget(this->menuView);
    setLayout(mainLayout);
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

    QObject::connect(this->textBox, &QLineEdit::textChanged, this, [this](){
        this->menuProxyModel->setFilterWildcard(this->textBox->text());
        this->selectRow(0);
    });
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
            this->selectRow(index.row() + 1);
            return true;
        }
        else if (ke->key() == Qt::Key_Up)
        {
            QModelIndex index = this->menuView->currentIndex();
            this->selectRow(index.row() - 1);
            return true;
        }
    }
    else if (e->type() == QEvent::ShortcutOverride)
    {
        QKeyEvent *ke = static_cast<QKeyEvent *>(e);
        if (ke->key() == Qt::Key_Tab)
        {
            this->textBox->setText(this->menuView->currentIndex().data(Qt::DisplayRole).toString());
            // this->textbox->setText(this->menu->currentItem()->text());
            this->textBox->setFocus(Qt::OtherFocusReason);
            this->textBox->grabKeyboard();
            e->accept();
            return true;
        }
    }
    return QFrame::event(e);
}

void
Dmenu::selectRow(int row)
{
    QModelIndex index = this->menuProxyModel->index(row, 0);
    if (index.isValid())
        this->menuView->setCurrentIndex(index);
}

void
Dmenu::focusOutEvent(QFocusEvent *e)
{
    e->accept();
    this->setFocus();
}

void
Dmenu::addEntry(QStringList entry)
{
    this->menuModel->setStringList(entry);
    // for (int i = 0; i < entry.size(); i++)
    // {
    //     this->menuModel->appendRow(new QStandardItem(entry.at(i)));
    // }
    // std::cout << entry.toStdString() << std::endl;
}

void
Dmenu::startThread()
{
    StdinReader *st = new StdinReader;
    st->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, st, &QObject::deleteLater);
    connect(this, &Dmenu::readStdin, st, &StdinReader::readStdin);
    connect(st, &StdinReader::newEntry, this, &Dmenu::addEntry);
    workerThread.start();
    emit readStdin();
}

Dmenu::~Dmenu()
{
    workerThread.quit();
    workerThread.wait();
}

void
Dmenu::slotStartThread()
{
    this->startThread();
}