#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "core/usersmodel.h"
#include "common.h"

#include <QTcpSocket>
#include <QMessageBox>

MainWindow::MainWindow(QTcpSocket *socket,
        const QString &username,
        QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _socket(socket),
    _username(username)
{
    ui->setupUi(this);

    _usersModel = new UsersModel(username, this);
    ui->lvUsers->setModel(_usersModel);

    connect(_socket, &QTcpSocket::disconnected, this, &MainWindow::onSocketDisconnected);
    connect(_socket, &QTcpSocket::readyRead, this, &MainWindow::onSocketReadyRead);
    connect(_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
    onSocketReadyRead();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addUnhandledMessage(const QString &message)
{
    handleRequest(message);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(_socket->state() == QTcpSocket::ConnectedState)
        _socket->disconnectFromHost();
    QMainWindow::closeEvent(event);
}

void MainWindow::onSocketError(QAbstractSocket::SocketError error)
{
    if(!_socketErrors)
        return;

    QString errorMessage;
    if(!_socketErrors->contains(error))
        errorMessage = (*_socketErrors)[QAbstractSocket::UnknownSocketError];
    else
        errorMessage = (*_socketErrors)[error];
    QMessageBox::critical(this, tr("Error"), errorMessage);
    qDebug() << "Socket Error: " << error << " " << errorMessage;
}

void MainWindow::onSocketDisconnected()
{
    close();
}

void MainWindow::onSocketReadyRead()
{
    QString line;
    while(_socket->canReadLine())
    {
        line = QString::fromUtf8(_socket->readLine()).trimmed();
        handleRequest(line);
    }
}

void MainWindow::handleRequest(const QString &line)
{
    qDebug() << line;
    if(Chat::RegExp::REGEXP_USER_JOINED.indexIn(line) != -1)
    {
        const QString message = Chat::RegExp::REGEXP_USER_JOINED.cap(1);
        if(message != _username)
        {
            ui->lvUsers->setUpdatesEnabled(false);
            _usersModel->addUser(message);
            ui->lvUsers->setUpdatesEnabled(true);
        }
        return;
    }
    else if(Chat::RegExp::REGEXP_USER_DISCONNECTED.indexIn(line) != -1)
    {
        const QString message = Chat::RegExp::REGEXP_USER_DISCONNECTED.cap(1);
        ui->lvUsers->setUpdatesEnabled(false);
        _usersModel->removeUser(message);
        ui->lvUsers->setUpdatesEnabled(true);
        return;
    }
    else if(Chat::RegExp::REGEXP_USERS.indexIn(line) != -1)
    {
        const QString message = Chat::RegExp::REGEXP_USERS.cap(1);
        ui->lvUsers->setUpdatesEnabled(false);
        _usersModel->init(message.split(';', QString::SkipEmptyParts));
        ui->lvUsers->setUpdatesEnabled(true);
        return;
    }
    else
    {
        ui->teChat->append(line);
    }
}

void MainWindow::on_pbSend_clicked()
{
    auto message = ui->leMessage->text();
    if(!message.isEmpty())
    {
        sendMessage(message);
        ui->leMessage->clear();
    }
}

void MainWindow::sendMessage(const QString &message)
{
    if(_socket && !message.isEmpty())
    {
        _socket->write((message + '\n').toUtf8());
    }
}

void MainWindow::on_leMessage_returnPressed()
{
    on_pbSend_clicked();
}
