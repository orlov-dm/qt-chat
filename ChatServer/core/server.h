#ifndef SERVER_H
#define SERVER_H
#include <QtNetwork/QTcpServer>
#include <QtCore/QHash>
#include <QtCore/QRegExp>

#include <functional>

class Connection;

class Server : public QTcpServer
{
    Q_OBJECT

public:
    static Server& getInstance() {
        static Server instance;
        return instance;
    }    
protected:
    virtual void incomingConnection(qintptr socketDescriptor) override;
private slots:
    void onReadyRead();
    void onDisconnected();
private:
    Server(QObject *parent = nullptr);
    Server(Server const&)               = delete;
    void operator=(Server const&)  = delete;

    void handleClientRequest(Connection *client, const QString &request);
    bool isClientAuthorized(Connection *client);

    void sendMessage(const QString &message, Connection *client = nullptr);
    void sendUsersList(Connection *client);

    //request handlers
    bool clientJoined(Connection *client, const QString &request);
    bool clientSentMessage(Connection *client, const QString &request);

    QList<std::function<bool(Connection *, const QString&)>> _requestHandlers;
    QHash<QString, Connection*> _clients;         
};

#endif // SERVER_H
