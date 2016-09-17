#include "desktopformathelper.h"

#include <QTextCodec>
#include <QDebug>
#include <QFile>

#include "utils.h"

#define KEY_NAME    "Name"
#define KEY_COMMENT "Comment"
#define KEY_ICON    "Icon"
#define KEY_EXEC    "Exec"
#define KEY_TYPE    "Type"

DesktopFormatHelper::DesktopFormatHelper()
{
}

// Load DesktopItem from .desktop file

bool DesktopFormatHelper::load (DesktopItem &item, const QString &fname)
{
    qDebug () << Q_FUNC_INFO << fname;

    QFile fd(fname);

    if (fd.open(QIODevice::ReadOnly))
    {
        QByteArray data = fd.readAll();

        QHash <QString,QString> hash = parse (data);

        QString name = hash[keyWithLang(KEY_NAME)];
        if (name.isEmpty()) name = hash[KEY_NAME];

        QString comment = hash[keyWithLang(KEY_COMMENT)];
        if (comment.isEmpty()) comment = hash[KEY_COMMENT];

        qDebug () << Q_FUNC_INFO << name;

        item.setName (name + "\n" + comment);
        item.setIcon (hash[KEY_ICON]);
        item.setPath (hash[KEY_EXEC]);

        return true;
    }

    return false;
}

QString DesktopFormatHelper::keyWithLang (const QString &key)
{
    QString lang = Utils::systemLanguage();
    return key + "[" + lang + "]";
}

QStringList DesktopFormatHelper::makeKeys ()
{
    QString lang = QLocale::system().name();
    qDebug () << Q_FUNC_INFO << lang;

    QStringList keys;
    keys << KEY_NAME
         << keyWithLang(KEY_NAME)
         << KEY_COMMENT
         << keyWithLang(KEY_COMMENT)
         << KEY_ICON
         << KEY_TYPE
         << KEY_EXEC;

    for (int i = 0; i < keys.size (); i++)
    {
        keys[i] += "=";
    }

    return keys;
}

QHash<QString,QString> DesktopFormatHelper::parse (const QByteArray &data)
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QString string = codec->toUnicode(data);

    QStringList strList = string.split ("\n");

    qDebug () << Q_FUNC_INFO << strList;

    QStringList keys = makeKeys ();

    QHash<QString,QString> hash;

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

                    if ( ! hash.contains(key)) hash[key] = str;

                    break;
                }
            }
        }
    }

    return hash;
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
