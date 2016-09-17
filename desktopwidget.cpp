#include "desktopwidget.h"
#include "ui_desktopwidget.h"

#include <QDebug>
#include <QToolBar>
#include <QMouseEvent>
#include <QSettings>
#include <QFile>
#include <QMenu>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>

#include "desktopwidgetdialog.h"
#include "utils.h"

DesktopWidget::DesktopWidget(QWidget *parent) :
    QWidget(parent), desktopMode_ (true),
    ui(new Ui::DesktopWidget)
{
    ui->setupUi(this);

    readSettings ();

    setDesktopWidgetFlags ();

    createView ();
}

//Items
void DesktopWidget::addItem (const QString &fname)
{
    DesktopItem desktopItem;
    desktopItem.load (fname);

    items.push_back(desktopItem);
}

void DesktopWidget::removeItems ()
{
    int size = items.size();
    for (int i=0; i < size; i++) items.pop_back();
}

//View
void DesktopWidget::createView ()
{
    iconBar_ = new QToolBar (this);

    ui->verticalLayout->insertWidget(0, iconBar_);

    connect (iconBar_, SIGNAL(actionTriggered(QAction*)), SLOT(executeApplicationSlot(QAction*)));

    reloadItems ();

    reloadIconBar ();
}

void DesktopWidget::reloadItems ()
{
    removeItems ();

    //itemPaths_ << "/usr/share/applications/gimp.desktop" << "/usr/share/applications/qtcreator.desktop" << "/usr/share/applications/firefox.desktop" ;

    QStringList itemPaths = settings_.getItems();

    for (int i = 0; i < itemPaths.size(); i++) addItem (itemPaths[i]);
}

#define ICON_COEFFICIENT 1.1

int DesktopWidget::getPanelWidth ()
{
    return settings_.getIconSize() * (items.size() + 1) * ICON_COEFFICIENT;
}

int DesktopWidget::getPanelHeight()
{
    return settings_.getIconSize() * ICON_COEFFICIENT;
}

int DesktopWidget::getWidth()
{
    if ( ! settings_.getDirection()) return getPanelWidth ();

    return getPanelHeight ();
}

int DesktopWidget::getHeight()
{
    if (! settings_.getDirection()) return getPanelHeight ();

    return getPanelWidth ();
}

void DesktopWidget::hovered ()
{
    qDebug () << Q_FUNC_INFO << "hovered";
}

void DesktopWidget::reloadIconBar ()
{
    int iconSize = settings_.getIconSize();

    QSize barSize = QSize (iconSize, iconSize);

    iconBar_->clear ();

    iconBar_->setIconSize (barSize);

    iconBar_->setOrientation(Qt::Orientation (settings_.getDirection() + 1));

    for (int i = 0 ; i < items.size(); i++)
    {
        actions_[i] = iconBar_->addAction (getResizedIcon (items[i].getIcon(), barSize), items[i].getName());

        //connect (actions_[i] , SIGNAL(hovered()), SLOT(hovered()));
    }

    resize (QSize (getWidth (), getHeight ()));
}

DesktopWidget::~DesktopWidget()
{
    removeItems ();

    delete ui;
}

//////////////////move by all point//////////////////
void DesktopWidget::mousePressEvent (QMouseEvent *pe)
{
    pos_ = pe->pos();
}

void DesktopWidget::mouseMoveEvent (QMouseEvent *pe)
{
    if (settings_.isPanelLocked()) return;

    move (pe->globalPos() - pos_);
}

//Settings

void DesktopWidget::readSettings ()
{
    //qDebug () << Q_FUNC_INFO;

    settings_.load();

    QPoint pos (0,50);
    QSize size (600,100);

    readWindowSettings (pos, size, objectName());

    //qDebug () << size.width ();

    setGeometry (pos.x(), pos.y(), size.width (), size.height ());
}

void DesktopWidget::writeSettings ()
{
    settings_.save();

    //qDebug () << Q_FUNC_INFO;

    writeWindowSettings (pos (), size (), objectName());

    qDebug () << Q_FUNC_INFO << width() << " " << height();
}

//DesktopWidget
void DesktopWidget::setDesktopWidgetFlags ()
{
    setDesktopWidget (desktopMode_);

    setTransparentBackground (desktopMode_);

    Qt::WindowFlags flags = 0;
    flags |= Qt::FramelessWindowHint;
    flags |= Qt::Tool;
    flags |= Qt::WindowStaysOnBottomHint;

    setWindowFlags(flags);
}

void DesktopWidget::setDesktopWidget (bool mode)
{
    setAttribute(Qt::WA_X11NetWmWindowTypeDesktop, mode);
}

void DesktopWidget::setTransparentBackground (bool mode)
{
    setAttribute(Qt::WA_TranslucentBackground, mode);
}

//Slots
void DesktopWidget::aboutSlot ()
{
    QMessageBox::about (this, tr("About"), QApplication::applicationName());
}

void DesktopWidget::settingsSlot ()
{
    DesktopWidgetDialog *dialog = new DesktopWidgetDialog(this);

    dialog->setWindowTitle (tr("Settings"));

    dialog->setInfo (settings_);

    if (dialog->exec () == QDialog::Accepted)
    {
        Settings info = dialog->getInfo ();

        settings_ = info;

        reloadItems();

        reloadIconBar ();

        writeSettings();

        setDesktopWidgetFlags ();

        hide ();
        show ();
    }

    delete dialog;
}

void DesktopWidget::executeApplication (const QString &path)
{
    process_.start (path);
}

void DesktopWidget::executeApplicationSlot (QAction *action)
{
    qDebug () << Q_FUNC_INFO << action->text();

    QString path;

    for (int i=0; i < MAX_ACTION; i++)
    {
        if (actions_[i] == action)
        {
            path = items[i].getPath();
            break;
        }
    }

    if (!path.isEmpty())
    {
        int len = path.length();
        int ind = path.indexOf (" ");
        if (ind != -1) path.remove (ind, len - ind);

        executeApplication (path);
    }
    else
        qDebug () << Q_FUNC_INFO << "No execute path!";
}

void DesktopWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);

    QAction *settingsAction = menu.addAction(tr("Settings"));
    connect (settingsAction, SIGNAL(triggered()), SLOT(settingsSlot()));

    //QAction *aboutAct = menu.addAction(tr("About"));
    //connect (aboutAct, SIGNAL(triggered()), SLOT(aboutSlot()));

    menu.addSeparator();

    QAction *exitAction = menu.addAction(tr("Exit"));
    connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    menu.exec (event->globalPos());
}

int count = 0;
void DesktopWidget::paintEvent(QPaintEvent *pe)
{
    /*
    QPainter p (this);

    p.setPen (Qt::transparent);
    p.setBrush (QColor (127, 127, 127, 127));

    p.drawRect (0, 0, pe->rect().width (), pe->rect().height ());

    qDebug () << Q_FUNC_INFO << "paint" << count;

    count++;
    */
}

void DesktopWidget::closeEvent (QCloseEvent *)
{
    writeSettings ();
}
