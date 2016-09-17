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
    QWidget(parent), desktopMode_ (true), iconSize_(64), direction_(0),
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

    for (int i=0; i < itemPaths_.size(); i++) addItem (itemPaths_[i]);
}

#define ICON_COEFFICIENT 1.1

int DesktopWidget::getPanelWidth ()
{
    return iconSize_ * (items.size() + 1) * ICON_COEFFICIENT;
}

int DesktopWidget::getPanelHeight()
{
    return iconSize_ * ICON_COEFFICIENT;
}

int DesktopWidget::getWidth()
{
    if (!direction_) return getPanelWidth ();
    else return getPanelHeight ();
}

int DesktopWidget::getHeight()
{
    if (!direction_) return getPanelHeight ();
    else return getPanelWidth ();
}

void DesktopWidget::hovered ()
{
    qDebug () << Q_FUNC_INFO << "hovered";
}

void DesktopWidget::reloadIconBar ()
{
    QSize barSize = QSize (iconSize_, iconSize_);

    iconBar_->clear ();

    iconBar_->setIconSize (barSize);

    iconBar_->setOrientation(Qt::Orientation (direction_ + 1));

    for (int i=0 ; i < items.size(); i++)
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
    move (pe->globalPos() - pos_);
}

//Settings

#define KEY_ICON_SIZE_STRING "icon-size"
#define KEY_DIRECTION_STRING "direction"
#define KEY_ITEMS_LIST_STRING "items-list"

void DesktopWidget::readSettings ()
{
    //qDebug () << Q_FUNC_INFO;

    QPoint pos (0,50);
    QSize size (600,100);

    readWindowSettings (pos, size, objectName());

    QSettings s;

    iconSize_ = s.value (KEY_ICON_SIZE_STRING, 64).toInt();

    direction_ = s.value (KEY_DIRECTION_STRING, 0).toBool();

    itemPaths_ = s.value (KEY_ITEMS_LIST_STRING, QStringList()).toStringList();

    if (itemPaths_.isEmpty()) itemPaths_ << ":/qIconPanel.desktop";

    //qDebug () << size.width ();

    setGeometry (pos.x(), pos.y(), size.width (), size.height ());
}

void DesktopWidget::writeSettings ()
{
    //qDebug () << Q_FUNC_INFO;

    writeWindowSettings (pos (), size (), objectName());

    QSettings s;

    s.setValue (KEY_ICON_SIZE_STRING, iconSize_);

    s.setValue (KEY_DIRECTION_STRING, direction_);

    s.setValue (KEY_ITEMS_LIST_STRING, itemPaths_);

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

    QVariantHash info;

    setIconSizeToHash (info, iconSize_);
    setDirectionToHash (info, direction_);
    setItemsToHash (info, itemPaths_);

    dialog->setInfo (info);

    if (dialog->exec () == QDialog::Accepted)
    {
        info = dialog->getInfo ();

        iconSize_ = getIconSizeFromHash (info);
        direction_ = getDirectionFromHash (info);
        itemPaths_ = getItemsFromHash (info);

        qDebug () << Q_FUNC_INFO << itemPaths_;

        reloadItems();

        reloadIconBar ();

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

    QAction *settingsAct = menu.addAction(tr("Settings"));
    connect (settingsAct, SIGNAL(triggered()), SLOT(settingsSlot()));

    //QAction *aboutAct = menu.addAction(tr("About"));
    //connect (aboutAct, SIGNAL(triggered()), SLOT(aboutSlot()));

    menu.addSeparator();

    QAction *exitAct = menu.addAction(tr("Exit"));
    connect (exitAct, SIGNAL(triggered()), SLOT(close()));

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

//Hash
//icon-size
int DesktopWidget::getIconSizeFromHash (const QVariantHash &hash)
{
    return hash.value(KEY_ICON_SIZE_STRING).toInt ();
}

void DesktopWidget::setIconSizeToHash (QVariantHash &hash, int iconSize)
{
    setValueToHash (hash, KEY_ICON_SIZE_STRING, iconSize);
}

//direction
bool DesktopWidget::getDirectionFromHash (const QVariantHash &hash)
{
    return hash.value(KEY_DIRECTION_STRING).toBool ();
}

void DesktopWidget::setDirectionToHash (QVariantHash &hash, bool direction)
{
    setValueToHash (hash, KEY_DIRECTION_STRING, direction);
}

//stringlist
QStringList DesktopWidget::getItemsFromHash (const QVariantHash &hash)
{
    return hash.value(KEY_ITEMS_LIST_STRING).toStringList();
}

void DesktopWidget::setItemsToHash (QVariantHash &hash, const QStringList &list)
{
    setValueToHash (hash, KEY_ITEMS_LIST_STRING, list);
}
