#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "core/server.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ipRegex (QString("^(%1\\.%2\\.%3\\.%4)|localhost$").arg(ipRange, ipRange, ipRange, ipRange));

    auto ipValidator = new QRegExpValidator(ipRegex, ui->leIPAddress);
    ui->leIPAddress->setValidator( ipValidator );    
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbStartServer_clicked()
{
    auto validator = ui->leIPAddress->validator();
    auto ipString = ui->leIPAddress->text();
    if(ipString.isEmpty())
    {
        QMessageBox::critical(this, tr("Error"), tr("IP is empty"));
        return;
    }
    int invalidPosition;
    if(!validator
            || validator->validate(ipString, invalidPosition) != QValidator::Acceptable)
    {
        QMessageBox::critical(this, tr("Error"), tr("IP is invalid"));
        return;
    }
    auto port = ui->sbPort->value();
    if(!port) {
        port = Server::DEFAULT_PORT;
        QMessageBox::warning(this, tr("Warning"), tr("Port is 0. Will be using default port"));
        ui->sbPort->setValue(port);
    }
    QHostAddress hostAddress(ipString);
    if(Server::getInstance().listen(hostAddress, port))
    {
        QMessageBox::information(this, tr("Information"), tr("Server successfully started: %1:%2").arg(ipString).arg(port));
        ui->pbStartServer->setEnabled(false);
        ui->pbStopServer->setEnabled(true);
    }
    else
    {
        QMessageBox::critical(this, tr("Error"), tr("Server cannot be started: %1:%2").arg(ipString).arg(port));
    }
}

void MainWindow::on_pbStopServer_clicked()
{
    if(Server::getInstance().isListening())
    {
        Server::getInstance().close();
        QMessageBox::information(this, tr("Information"), tr("Server successfully stopped"));
        ui->pbStartServer->setEnabled(true);
        ui->pbStopServer->setEnabled(false);
    }
}
