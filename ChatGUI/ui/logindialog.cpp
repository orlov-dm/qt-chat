#include "logindialog.h"
#include "ui_logindialog.h"
#include "mainwindow.h"
#include "common.h"
#include <QtNetwork/QTcpSocket>
#include <QMessageBox>
#include <QSettings>

#include <QDebug>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    _socket = new QTcpSocket(this);
    const int WAIT_TIME = 5000;
    _socket->waitForConnected(WAIT_TIME);
    _socket->waitForReadyRead(WAIT_TIME);
    connect(_socket, &QTcpSocket::connected, this, &LoginDialog::onSocketConnected);    
    connect(_socket, &QTcpSocket::disconnected, this, &LoginDialog::onSocketDisconnected);
    connect(_socket, &QTcpSocket::readyRead, this, &LoginDialog::onSocketReadyRead);
    connect(_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));

    refreshInfo(false);

    ui->leIPAddress->setValidator( Chat::makeIPValidator(ui->leIPAddress) );

    restoreData();

}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_pbConnect_clicked()
{
    auto validator = ui->leIPAddress->validator();
    auto ipString = ui->leIPAddress->text();
    auto port = ui->sbPort->value();

    if(!port) {
        port = Chat::DEFAULT_PORT;
        QMessageBox::warning(this, tr("Warning"), tr("Port is 0. Will be using default port"));
        ui->sbPort->setValue(port);
    }
    auto username = ui->leUsername->text();   
    if(checkConnectInfo(ipString, validator, username))
    {
        if(ipString == "localhost")
            ipString = "127.0.0.1";
        _socket->connectToHost(ipString, port);
        refreshInfo(true);
    }       
}

void LoginDialog::on_pbStop_clicked()
{
    _socket->abort();
    refreshInfo(false);
}

void LoginDialog::onSocketConnected()
{
    qDebug() << "Socket Connected";
    sendMessage(QString("%1%2").arg(Chat::Messages::START).arg(ui->leUsername->text()));
}

void LoginDialog::onSocketError(QAbstractSocket::SocketError error)
{
    QString errorMessage;
    if(!_socketErrors.contains(error))
        errorMessage = _socketErrors[QAbstractSocket::UnknownSocketError];
    else
        errorMessage = _socketErrors[error];
    QMessageBox::critical(this, tr("Error"), errorMessage);
    qDebug() << "Socket Error: " << error << " " << errorMessage;
    refreshInfo(false);
}

void LoginDialog::onSocketDisconnected()
{
    qDebug() << "Socket Disconnected";
    refreshInfo(false);
}

void LoginDialog::onSocketReadyRead()
{
    QString line;
    if(_socket->canReadLine())
    {        
        line = QString::fromUtf8(_socket->readLine()).trimmed();
        qDebug() << line;
        if(Chat::RegExp::REGEXP_USER_BUSY.indexIn(line) != -1)
        {
            const QString message = Chat::RegExp::REGEXP_USER_BUSY.cap(1);
            QMessageBox::critical(this, tr("Error"), tr("User is already logged on"));
            _socket->disconnectFromHost();
            return;
        }
        startChatWindow(line);
        refreshInfo(false);
        saveData();
    }
}


void LoginDialog::sendMessage(const QString &message)
{
    if(_socket && !message.isEmpty())
    {
        _socket->write((message + '\n').toUtf8());
    }
}

bool LoginDialog::checkConnectInfo(QString &ipString, const QValidator *validator, const QString &username)
{
    if(ipString.isEmpty())
    {
        QMessageBox::critical(this, tr("Error"), tr("IP is empty"));
        return false;
    }
    int invalidPosition;
    if(!validator
            || validator->validate(ipString, invalidPosition) != QValidator::Acceptable)
    {
        QMessageBox::critical(this, tr("Error"), tr("IP is invalid"));
        return false;
    }
    if(username.isEmpty())
    {
        QMessageBox::critical(this, tr("Error"), tr("Username is empty"));
        return false;
    }
    return true;
}

void LoginDialog::startChatWindow(const QString &line)
{
    _chatWindow = _chatWindow ? _chatWindow : new MainWindow(_socket, ui->leUsername->text(), this);
    _chatWindow->addUnhandledMessage(line);
    _chatWindow->setWindowModality(Qt::ApplicationModal);
    _chatWindow->show();
}

void LoginDialog::refreshInfo(bool isConnecting)
{
    ui->pbConnect->setEnabled(!isConnecting);
    ui->pbStop->setEnabled(isConnecting);
    if(isConnecting)
        ui->lInfo->setText(tr("Trying to connect..."));
    else
        ui->lInfo->setText("");
}

void LoginDialog::initSocketErrors()
{
    _socketErrors[QAbstractSocket::ConnectionRefusedError] = tr("The connection was refused by the peer (or timed out).");
    _socketErrors[QAbstractSocket::RemoteHostClosedError] = tr("The remote host closed the connection. Note that the client socket (i.e., this socket) will be closed after the remote close notification has been sent.");
    _socketErrors[QAbstractSocket::HostNotFoundError] = tr("The host address was not found.");
    _socketErrors[QAbstractSocket::SocketAccessError] = tr("The socket operation failed because the application lacked the required privileges.");
    _socketErrors[QAbstractSocket::SocketResourceError] = tr("The local system ran out of resources (e.g., too many sockets).");
    _socketErrors[QAbstractSocket::SocketTimeoutError] = tr("The socket operation timed out.");
    _socketErrors[QAbstractSocket::DatagramTooLargeError] = tr("The datagram was larger than the operating system's limit (which can be as low as 8192 bytes).");
    _socketErrors[QAbstractSocket::NetworkError] = tr("An error occurred with the network (e.g., the network cable was accidentally plugged out).");
    _socketErrors[QAbstractSocket::AddressInUseError] = tr("The address specified to QAbstractSocket::bind() is already in use and was set to be exclusive.");
    _socketErrors[QAbstractSocket::SocketAddressNotAvailableError] = tr("The address specified to QAbstractSocket::bind() does not belong to the host.");
    _socketErrors[QAbstractSocket::UnsupportedSocketOperationError] = tr("The requested socket operation is not supported by the local operating system (e.g., lack of IPv6 support).");
    _socketErrors[QAbstractSocket::ProxyAuthenticationRequiredError] = tr("The socket is using a proxy, and the proxy requires authentication.");
    _socketErrors[QAbstractSocket::SslHandshakeFailedError] = tr("The SSL/TLS handshake failed, so the connection was closed (only used in QSslSocket)");
    _socketErrors[QAbstractSocket::UnfinishedSocketOperationError] = tr("Used by QAbstractSocketEngine only, The last operation attempted has not finished yet (still in progress in the background).");
    _socketErrors[QAbstractSocket::ProxyConnectionRefusedError] = tr("Could not contact the proxy server because the connection to that server was denied");
    _socketErrors[QAbstractSocket::ProxyConnectionClosedError] = tr("The connection to the proxy server was closed unexpectedly (before the connection to the final peer was established)");
    _socketErrors[QAbstractSocket::ProxyConnectionTimeoutError] = tr("The connection to the proxy server timed out or the proxy server stopped responding in the authentication phase.");
    _socketErrors[QAbstractSocket::ProxyNotFoundError] = tr("The proxy address set with setProxy() (or the application proxy) was not found.");
    _socketErrors[QAbstractSocket::ProxyProtocolError] = tr("The connection negotiation with the proxy server failed, because the response from the proxy server could not be understood.");
    _socketErrors[QAbstractSocket::OperationError] = tr("An operation was attempted while the socket was in a state that did not permit it.");
    _socketErrors[QAbstractSocket::SslInternalError] = tr("The SSL library being used reported an internal error. This is probably the result of a bad installation or misconfiguration of the library.");
    _socketErrors[QAbstractSocket::SslInvalidUserDataError] = tr("Invalid data (certificate, key, cypher, etc.) was provided and its use resulted in an error in the SSL library.");
    _socketErrors[QAbstractSocket::TemporaryError] = tr("A temporary error occurred (e.g., operation would block and socket is non-blocking).");
    _socketErrors[QAbstractSocket::UnknownSocketError] = tr("An unidentified error occurred.");
}

void LoginDialog::saveData()
{
    QSettings settings("deo", "Chat GUI");
    settings.setValue(REG_SERVER_ADDR, ui->leIPAddress->text());
    settings.setValue(REG_SERVER_PORT, ui->sbPort->value());
    settings.setValue(REG_USERNAME, ui->leUsername->text());
}

void LoginDialog::restoreData()
{
    QSettings settings("deo", "Chat GUI");
    auto addr = settings.value(REG_SERVER_ADDR).toString();
    if(!addr.isEmpty())
        ui->leIPAddress->setText(addr);
    bool ok;
    auto port = settings.value(REG_SERVER_PORT).toInt(&ok);
    if(ok)
        ui->sbPort->setValue(port);
    auto username = settings.value(REG_USERNAME).toString();
    if(!username.isEmpty())
        ui->leUsername->setText(username);
}


