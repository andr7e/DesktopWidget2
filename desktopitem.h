#ifndef DESKTOPITEM_H
#define DESKTOPITEM_H

#include <QString>

class DesktopItem
{
    QString name_;
    QString icon_;
    QString path_;

public:
    DesktopItem (){}
    DesktopItem (const QString &name, const QString &icon, const QString &path) : name_(name), icon_(icon), path_(path){}

    QString getName () const;
    QString getIcon () const;
    QString getPath () const;

    void setName (const QString &name);
    void setIcon (const QString &icon);
    void setPath (const QString &path);

    void load (const QString &fname);
};

#endif // DESKTOPITEM_H
