#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QRegExp>
#include <QRegExpValidator>

namespace Chat {
    const int DEFAULT_PORT = 20000;
    QRegExpValidator *makeIPValidator(QObject *parent);

    namespace Messages {
        const QString START = "/start:";
        const QString USERS = "/users:";
        const QString USER_BUSY = "/user-busy:";
        const QString USER_JOINED = "/user-join:";
        const QString USER_DISCONNECTED = "/user-out:";
    }

    namespace RegExp {
        QRegExp makeRegExp(const QString &message);

        const QRegExp REGEXP_START = makeRegExp(Chat::Messages::START);
        const QRegExp REGEXP_USERS = makeRegExp(Chat::Messages::USERS);
        const QRegExp REGEXP_USER_BUSY = makeRegExp(Chat::Messages::USER_BUSY);
        const QRegExp REGEXP_USER_JOINED = makeRegExp(Chat::Messages::USER_JOINED);
        const QRegExp REGEXP_USER_DISCONNECTED = makeRegExp(Chat::Messages::USER_DISCONNECTED);
    }
}



#endif // COMMON_H
