#ifndef SETTINGS_H
#define SETTINGS_H

#include <QStringList>

class Settings
{
public:
    enum Direction
    {
        HORIZONTAL = 0,
        VERTICAL
    };

    Settings();

    Settings (const Settings &obj);

    int getIconSize() const;
    int getDirection() const;
    bool isPanelLocked() const;
    QStringList getItems() const;

    void setIconSize(int size);
    void setDirection(int direction);
    void setPanelLocked(bool lock);
    void setItems(const QStringList &items);

    void load();
    void save();

private:
    int iconSize_;
    int direction_;
    bool lockPanel_;
    QStringList itemPaths_;
};

#endif // SETTINGS_H
