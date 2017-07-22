#ifndef COMMON_H
#define COMMON_H

#include <QString>

namespace Chat {
    const int DEFAULT_PORT = 10000;
    QRegExpValidator *makeIPValidator(QObject *parent)
    {
        const QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
        const QRegExp ipRegex (QString("^(%1\\.%2\\.%3\\.%4)|localhost$").arg(ipRange, ipRange, ipRange, ipRange));

        return new QRegExpValidator(ipRegex, parent);
    }
}





#endif // COMMON_H
