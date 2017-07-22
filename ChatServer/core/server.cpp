#include "server.h"
#include "core/connection.h"
#include "constants.h"
#include <QDebug>

#include <cassert>

Server::Server(QObject *parent):QTcpServer(parent)
{
    _requestHandlers = {
        std::bind(&Server::clientJoined, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&Server::clientSentMessage, this, std::placeholders::_1, std::placeholders::_2)
    };
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    auto client = new Connection(this);
    client->setSocketDescriptor(socketDescriptor);

    qDebug() << "New client from:" << client->peerAddress().toString();

    connect(client, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(client, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
}

void Server::onReadyRead()
{
    auto client = static_cast<Connection*>(sender());
    while(client->canReadLine())
    {
        auto line = QString::fromUtf8(client->readLine()).trimmed();
        handleClientRequest(client, line);
    }
}

void Server::onDisconnected()
{
    auto client = static_cast<Connection*>(sender());
    assert(client);
    qDebug() << "Client disconnected:" << client->peerAddress().toString();

    _clients.remove(client->getClientName());

    sendUsersList();
    sendMessage(QString("Server:%1 has left.").arg(client->getClientName()));
}

void Server::handleClientRequest(Connection *client, const QString &request)
{
    assert(client);
    qDebug() << "Read line:" << request;

    bool requestHandled = false;
    for(auto handler: _requestHandlers) {
        if(handler(client, request)) {
            requestHandled = true;
            break;
        }
    }

    if(!requestHandled)
    {
        qWarning() << "Got bad message from client:" << client->peerAddress().toString() << " " << request;
    }
}

bool Server::isClientAuthorized(Connection *client)
{
    assert(client);
    return _clients.contains(client->getClientName());
}

void Server::sendMessage(const QString &message, Connection *client)
{
    if(!client)
    {
        for(auto client: _clients)
            client->write((message + '\n').toUtf8());
    }
    else
    {
        client->write((message + '\n').toUtf8());
    }
}

void Server::sendUsersList()
{
    QStringList clientNames;
    for(auto client: _clients)
        clientNames.append(client->getClientName());

    sendMessage(QString("%1%2").arg(Chat::Messages::USERS).arg(clientNames.join(';')));
}

bool Server::clientJoined(Connection *client, const QString &request)
{
    assert(client);
    bool handled = false;
    if(Chat::RegExp::REGEXP_START.indexIn(request) != -1)
    {
        const QString clientName = Chat::RegExp::REGEXP_START.cap(1);
        if(!_clients.contains(clientName))
        {
            client->setClientName(clientName);
            _clients.insert(clientName, client);
            sendMessage(QString("Server:%1 has joined.\n").arg(clientName));
            sendUsersList();
        }
        else
        {
            sendMessage(Chat::Messages::USER_BUSY);
        }
        handled = true;
    }
    return handled;
}

bool Server::clientSentMessage(Connection *client, const QString &request)
{
    assert(client);
    bool handled = false;
    if(isClientAuthorized(client))
    {
        const QString &message = request;
        qDebug() << "User:" << client->getClientName();
        qDebug() << "Message:" << message;

        sendMessage(QString("%1:%2").arg(client->getClientName()).arg(message));
        handled = true;
    }
    return handled;
}

