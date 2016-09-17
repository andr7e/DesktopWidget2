#include "desktopitem.h"

#include "desktopformathelper.h"

QString DesktopItem::getName() const
{
    return name_;
}

QString DesktopItem::getIcon() const
{
    return icon_;
}

QString DesktopItem::getPath() const
{
    return path_;
}

void DesktopItem::setName(const QString &name)
{
    name_ = name;
}

void DesktopItem::setIcon(const QString &icon)
{
    icon_ = icon;
}

void DesktopItem::setPath(const QString &path)
{
    path_ = path;
}

void DesktopItem::load (const QString &fname)
{
    DesktopFormatHelper::load(*this, fname);
}
