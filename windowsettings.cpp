#include "windowsettings.h"
#include <QSettings>
#include <QDebug>

WindowSettings::WindowSettings()
{
}

#define KEY_POS_STRING "pos"
#define KEY_SIZE_STRING "size"

#define KEY_WINDOW "Window"

void WindowSettings::readWindowSettings (QPoint &pos, QSize &size, const QString &key)
{
    //qDebug () << Q_FUNC_INFO;

    QSettings s;
    s.beginGroup (key + KEY_WINDOW);
    pos = s.value (KEY_POS_STRING, pos).toPoint();
    size = s.value (KEY_SIZE_STRING, size).toSize();
    s.endGroup();
}

void WindowSettings::writeWindowSettings (const QPoint &pos, const QSize &size, const QString &key)
{
    //qDebug () << Q_FUNC_INFO;

    QSettings s;
    s.beginGroup (key + KEY_WINDOW);
    s.setValue (KEY_POS_STRING, pos);
    s.setValue (KEY_SIZE_STRING, size);
    s.endGroup();
}
