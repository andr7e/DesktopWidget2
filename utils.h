#ifndef UTILS_H
#define UTILS_H

#include <QIcon>
#include <QVariantHash>

//Load from theme or file and resize icon
QIcon getResizedIcon (const QString &fname, const QSize &size);

void setValueToHash (QVariantHash &hash, const QString &key, const QVariant &value);

#endif // UTILS_H
