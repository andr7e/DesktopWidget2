#ifndef DESKTOPWIDGET_H
#define DESKTOPWIDGET_H

#include <QWidget>
#include <QProcess>
#include <QAction>
#include <QToolBar>

#include "windowsettings.h"
#include "desktopitem.h"

#define MAX_ACTION 128

#define APP_ICON_STRING ":/images/qiconpanel.png"

namespace Ui {
class DesktopWidget;
}


class DesktopWidget : public QWidget, public WindowSettings
{
    Q_OBJECT

protected:
    virtual void mousePressEvent (QMouseEvent *pe);
    virtual void mouseMoveEvent (QMouseEvent *pe);

    void readSettings ();
    void writeSettings ();

    void setDesktopWidgetFlags ();
    void setDesktopWidget (bool mode);
    void setTransparentBackground (bool mode);

    //events
    void contextMenuEvent(QContextMenuEvent *event);

    void paintEvent(QPaintEvent *pe);
    void closeEvent(QCloseEvent *);

public:
    explicit DesktopWidget(QWidget *parent = 0);
    ~DesktopWidget();

    void addItem (const QString &fname);
    void removeItems ();

    void createView ();
    void reloadItems ();
    void reloadIconBar ();

    int getPanelWidth ();
    int getPanelHeight();

    int getWidth ();
    int getHeight();

    void executeApplication (const QString &path);

    //Hash
    static int getIconSizeFromHash (const QVariantHash &hash);
    static void setIconSizeToHash (QVariantHash &hash, int iconSize);

    static bool getDirectionFromHash (const QVariantHash &hash);
    static void setDirectionToHash (QVariantHash &hash, bool direction);

    static QStringList getItemsFromHash (const QVariantHash &hash);
    static void setItemsToHash (QVariantHash &hash, const QStringList &list);

public slots:
    void executeApplicationSlot (QAction *action);

    void settingsSlot ();
    void aboutSlot ();

    void hovered ();
    
private:
    bool desktopMode_;
    QPoint pos_;
    QProcess process_;
    int iconSize_;
    bool direction_;
    QAction *actions_[MAX_ACTION];
    QToolBar *iconBar_;
    Ui::DesktopWidget *ui;

    QStringList itemPaths_;
    QVector<DesktopItem> items;
};

#endif // DESKTOPWIDGET_H
