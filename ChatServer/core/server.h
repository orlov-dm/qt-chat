#ifndef SERVER_H
#define SERVER_H
#include <QtNetwork/QTcpServer>
#include <QtCore/QHash>
#include <QtCore/QRegExp>

#include <constants.h>

#include <functional>

class Connection;

class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server(QObject *parent = nullptr);
protected:
    virtual void incomingConnection(qintptr socketDescriptor) override;
private slots:
    void onReadyRead();
    void onDisconnected();
private:
    void handleClientRequest(Connection *client, const QString &request);
    bool isClientAuthorized(Connection *client);

    void sendMessage(const QString &message);
    void sendUsersList();

    //request handlers
    bool clientJoined(Connection *client, const QString &request);
    bool clientSentMessage(Connection *client, const QString &request);

    QList<std::function<bool(Connection *, const QString&)>> _requestHandlers;
    QHash<QString, Connection*> _clients;

    const QRegExp REGEXP_START = QRegExp(QString("^%1(.*)$").arg(Chat::Messages::START));
};

#endif // SERVER_H
