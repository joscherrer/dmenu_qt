#include "dmenu.hpp"
#include "textbox.hpp"
#include "stdin.hpp"
#include "config.hpp"
#include "helper.hpp"
#include "menuview.hpp"
#include <iostream>
#include <QFile>
#include <QFontDatabase>
#include <QVBoxLayout>
#include <QRegularExpression>


Dmenu::Dmenu(QWidget *parent)
:QFrame(parent, Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint)
{
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

    this->startThread();
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
        else if (ke->key() == Qt::Key_Left)
        {
        }
        else if (ke->key() == Qt::Key_Right)
        {

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
}

void
Dmenu::startThread()
{
    std::cout << "Started thread at " << h.time_from_start() << std::endl;
    StdinReader *st = new StdinReader;
    st->moveToThread(&workerThread);
    // connect(&workerThread, &QThread::finished, st, &QObject::deleteLater);
    connect(st, &StdinReader::stdinDone, this, [this](){
        std::cout << "Finished reading stdin in " << h.time_from_start() << std::endl;
    });
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