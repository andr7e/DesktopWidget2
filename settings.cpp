#include "settings.h"

#define KEY_ICON_SIZE_STRING  "icon-size"
#define KEY_DIRECTION_STRING  "direction"
#define KEY_ITEMS_LIST_STRING "items-list"
#define KEY_LOCK_PANEL_STRING  "lock-panel"

#include <QSettings>

Settings::Settings() : iconSize_(64), direction_(0), lockPanel_(false)
{
}

Settings::Settings(const Settings &obj)
{
    iconSize_  = obj.iconSize_;
    direction_ = obj.direction_;
    lockPanel_ = obj.lockPanel_;
    itemPaths_ = obj.itemPaths_;
}

int Settings::getIconSize() const
{
    return iconSize_;
}

int Settings::getDirection() const
{
    return direction_;
}

bool Settings::isPanelLocked() const
{
    return lockPanel_;
}

QStringList Settings::getItems() const
{
    return itemPaths_;
}

void Settings::setIconSize(int size)
{
    iconSize_ = size;
}

void Settings::setDirection(int direction)
{
    direction_ = direction;
}

void Settings::setPanelLocked(bool lock)
{
    lockPanel_ = lock;
}

void Settings::setItems(const QStringList &items)
{
    itemPaths_ = items;
}

void Settings::load()
{
    QSettings s;

    iconSize_ = s.value (KEY_ICON_SIZE_STRING, 64).toInt();

    direction_ = s.value (KEY_DIRECTION_STRING, 0).toBool();

    itemPaths_ = s.value (KEY_ITEMS_LIST_STRING, QStringList()).toStringList();

    lockPanel_ = s.value (KEY_LOCK_PANEL_STRING, 0).toBool();

    if (itemPaths_.isEmpty()) itemPaths_ << ":/qIconPanel.desktop";
}

void Settings::save()
{
    QSettings s;

    s.setValue (KEY_ICON_SIZE_STRING, iconSize_);

    s.setValue (KEY_DIRECTION_STRING, direction_);

    s.setValue (KEY_ITEMS_LIST_STRING, itemPaths_);

    s.setValue (KEY_LOCK_PANEL_STRING, lockPanel_);
}
