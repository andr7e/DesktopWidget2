#include <QApplication>
#include <QBitmap>

#include "desktopwidget.h"

#define ORG_MAME "AEFR"
#define APP_MAME "DesktopWidget"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setOrganizationName (ORG_MAME);
    a.setApplicationName (APP_MAME);

    DesktopWidget w;
    w.show();
    
    return a.exec();
}
