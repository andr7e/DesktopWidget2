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

    QString getName () const { return name_; }
    QString getIcon () const { return icon_; }
    QString getPath () const { return path_; }

    void setName (const QString &name){ name_=name; }
    void setIcon (const QString &icon){ icon_=icon; }
    void setPath (const QString &path){ path_=path; }

    void load (const QString &fname);
};

#endif // DESKTOPITEM_H
