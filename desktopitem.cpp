#include "desktopitem.h"

#include <QFile>
#include <QHash>
#include <QDebug>
#include <QStringList>
#include <QTextCodec>

#define KEY_NAME "Name"
#define KEY_COMMENT "Comment"
#define KEY_ICON "Icon"
#define KEY_EXEC "Exec"
#define KEY_TYPE "Type"

QHash <QString,QString> parseDesktopFormat (const QByteArray &data);
QString systemLanguage ();

QString keyWithLang (const QString &key)
{
    return key + "[" + systemLanguage() + "]";
}

//Load DesktopItem from .desktop file
void DesktopItem::load (const QString &fname)
{
    qDebug () << Q_FUNC_INFO << fname;

    QFile fd(fname);

    if (fd.open(QIODevice::ReadOnly))
    {
        QByteArray data = fd.readAll();

        QHash <QString,QString> hash = parseDesktopFormat (data);

        QString name = hash[keyWithLang(KEY_NAME)];
        if (name.isEmpty()) name = hash[KEY_NAME];

        QString comment = hash[keyWithLang(KEY_COMMENT)];
        if (comment.isEmpty()) comment = hash[KEY_COMMENT];

        qDebug () << Q_FUNC_INFO << name;

        setName (name + "\n" + comment);
        setIcon (hash[KEY_ICON]);
        setPath (hash[KEY_EXEC]);
    }

}

/*
Example

[Desktop Entry]
Version=1.0
Type=Application
Name=GIMP Image Editor
GenericName=Image Editor
Comment=Create images and edit photographs
Exec=gimp-2.8 %U
TryExec=gimp-2.8
Icon=gimp
Terminal=false
Categories=Graphics;2DGraphics;RasterGraphics;GTK;
X-GNOME-Bugzilla-Bugzilla=GNOME
X-GNOME-Bugzilla-Product=GIMP
X-GNOME-Bugzilla-Component=General
X-GNOME-Bugzilla-Version=2.8.2
X-GNOME-Bugzilla-OtherBinaries=gimp-2.8
StartupNotify=true
MimeType=application/postscript;application/pdf;
*/

QString systemLanguage ()
{
    QString lang = QLocale::system().name();
    return (lang.remove(2, lang.size() - 2));
}

QStringList makeKeys ()
{
    QString lang = QLocale::system().name();
    qDebug () << Q_FUNC_INFO << (lang.remove(2, lang.size() - 2));

    QStringList keys;
    keys << KEY_NAME << KEY_COMMENT  << KEY_ICON << KEY_TYPE << KEY_EXEC << keyWithLang(KEY_NAME) << keyWithLang(KEY_COMMENT);

    for (int i=0; i < keys.size (); i++)
    {
        keys[i] += "=";
    }

    return keys;
}

QHash <QString,QString> parseDesktopFormat (const QByteArray &data)
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QString string = codec->toUnicode(data);

    QStringList strList = string.split ("\n");

    qDebug () << Q_FUNC_INFO << strList;

    QStringList keys = makeKeys ();

    QHash <QString,QString> hash;

    //look string list
    foreach (QString str, strList)
    {
        if (!str.isEmpty())
        {
            foreach (QString key, keys)
            {
                //key in str?
                if (str.startsWith (key))
                {
                    str.remove(key);

                    key.remove("=");

                    qDebug () << Q_FUNC_INFO << key << str;

                    if (!hash.contains(key)) hash[key] = str;

                    break;
                }
            }
        }
    }

    return hash;
}
