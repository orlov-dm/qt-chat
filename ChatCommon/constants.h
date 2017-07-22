#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>
namespace Chat {
    namespace Messages {
        const QString START = "/start:";
        const QString USERS = "/users:";
        const QString USER_BUSY = "/user-busy:";
    }

    namespace RegExp {
        QRegExp makeRegExp(const QString &message)
        {
            return QRegExp(QString("^%1(.*)$").arg(message));
        }

        const QRegExp REGEXP_START = makeRegExp(Chat::Messages::START);
        const QRegExp REGEXP_USERS = makeRegExp(Chat::Messages::USERS);
        const QRegExp REGEXP_USER_BUSY = makeRegExp(Chat::Messages::USER_BUSY);
    }
}
#endif // CONSTANTS_H
