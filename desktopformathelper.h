#ifndef DESKTOPFORMATHELPER_H
#define DESKTOPFORMATHELPER_H

#include <QHash>
#include <QByteArray>
#include <QStringList>

#include "desktopitem.h"

class DesktopFormatHelper
{
public:
    DesktopFormatHelper();

    static bool load (DesktopItem &item, const QString &fname);

    static QHash<QString,QString> parse (const QByteArray &data);

    static QStringList makeKeys ();

    static QString keyWithLang (const QString &key);
};

#endif // DESKTOPFORMATHELPER_H
