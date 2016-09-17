#ifndef WINDOWSETTINGS_H
#define WINDOWSETTINGS_H

#include <QPoint>
#include <QSize>
#include <QString>

class WindowSettings
{
public:
    WindowSettings();

    //Input - default value, output - set to input
    void readWindowSettings (QPoint &pos, QSize &size, const QString &key = 0);

    void writeWindowSettings (const QPoint &pos, const QSize &size, const QString &key = 0);
};

#endif // WINDOWSETTINGS_H
