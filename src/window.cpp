#include "window.hpp"

#include <iostream>
#include <QVBoxLayout>
#include <QRegularExpression>
#include <QSortFilterProxyModel>

Tester::Tester(QWidget* parent)
:QMainWindow(parent), mButtonPressedAmt(0)
{
    //Since this is a QMainWindow we cannot set a layout,
    //we can only set a central widget.
    auto centralWidget = new QWidget(this);

    this->mpLabel = new QLabel(centralWidget);
    this->mpPushButton = new QPushButton("Press Me!", centralWidget);

    //Make sure our label has the correct text
    //starting out.
    this->updateLabelText();

    //Since setting a layout on QMainWindow is not allowed,
    //this layout is used in centralWidget instead.
    auto mainLayout = new QVBoxLayout();
    mainLayout->addWidget(this->mpLabel);
    mainLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed));
    mainLayout->addWidget(this->mpPushButton);

    centralWidget->setLayout(mainLayout);
    this->setCentralWidget(centralWidget);

    //Syntax to connect a Qt signal with a lambda.
    QObject::connect(this->mpPushButton, &QPushButton::clicked, this, [this](){
        this->mButtonPressedAmt++;
        this->updateLabelText();
    });

    //Syntax to connect a custom class signal with a lambda.
    QObject::connect(this, &Tester::sigLabelTextUpdated, this, [this](std::string_view val){
        std::cout << val << std::endl;
    });
}

void Tester::updateLabelText()
{
    this->mpLabel->setText("Button was clicked: " + QString::number(this->mButtonPressedAmt) + " times.");

    emit sigLabelTextUpdated(this->mpLabel->text().toStdString());
}


Dmenu::Dmenu(QWidget* parent)
:QFrame(parent)
{
    this->textbox = new QLineEdit(this);
    this->menu = new QListWidget(this);
    this->menuModel = new QStandardItemModel(this);
    this->menuView = new QListView(this);
    this->menuProxyModel = new QSortFilterProxyModel(this);
    this->menuProxyModel->setSourceModel(this->menuModel);
    this->menuView->setModel(this->menuModel);

    this->textbox->setFocusPolicy(Qt::StrongFocus);

    auto mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(QMargins(0,0,0,0));
    mainLayout->addWidget(this->textbox);
    mainLayout->addWidget(this->menu);
    mainLayout->addWidget(this->menuView);
    setLayout(mainLayout);
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

    QObject::connect(this->textbox, &QLineEdit::textChanged, this, [this](){
        std::cout << this->textbox->text().toStdString() << std::endl;
    });
}

bool Dmenu::event(QEvent *e)
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
            if (this->menu->count() == 0)
                std::cout << this->textbox->text().toStdString() << std::endl;
            else
                std::cout << this->menu->currentItem()->text().toStdString() << std::endl;
            this->close();
            return true;
        }
        else if (ke->key() == Qt::Key_Down)
        {
            if (this->menu->currentRow() < this->menu->count() - 1)
                this->menu->setCurrentRow(this->menu->currentRow() + 1);
            return true;
        }
        else if (ke->key() == Qt::Key_Up)
        {
            if (this->menu->currentRow() > 0)
                this->menu->setCurrentRow(this->menu->currentRow() - 1);
            return true;
        }
    }
    else if (e->type() == QEvent::ShortcutOverride)
    {
        QKeyEvent *ke = static_cast<QKeyEvent *>(e);
        if (ke->key() == Qt::Key_Tab)
        {
            this->textbox->setText(this->menu->currentItem()->text());
            this->textbox->setFocus();
            this->textbox->grabKeyboard();
            return true;
        }
    }

    return QFrame::event(e);
}

void Dmenu::setMenu(QStringList data)
{
    QStringList::const_iterator constIter;
    for (constIter = data.constBegin(); constIter != data.constEnd(); ++constIter)
    {
        this->menuModel->appendRow(new QStandardItem((*constIter)));
    }
}

void Dmenu::oldSetMenu(QStringList data)
{
    this->menu->addItems(data);
}

void Dmenu::printMenu(QStringList data)
{
    QStringList::const_iterator constIter;
    for (constIter = data.constBegin(); constIter != data.constEnd(); ++constIter)
    {
        std::cout << (*constIter).toLocal8Bit().constData() << std::endl;
    }
}

void Dmenu::slotFilterMenu()
{
}