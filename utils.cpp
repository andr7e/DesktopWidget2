#include "utils.h"

#include <QLocale>

QIcon getResizedIcon (const QString &fname, const QSize &size)
{
    QIcon icon;
    if (!fname.contains("/"))
    {
        icon = QIcon::fromTheme(fname);
    }
    else
    {
        icon = QIcon (fname);
    }

    if (!icon.isNull())
    {
        QPixmap pixmap = icon.pixmap (size);
        icon = QIcon (pixmap);
    }

    return icon;
}

void setValueToHash (QVariantHash &hash, const QString &key, const QVariant &value)
{
    if (!hash.contains (key)) hash[key] = value;
}

QString Utils::systemLanguage()
{
    QString lang = QLocale::system().name();

    return lang.mid(0,2);
}
