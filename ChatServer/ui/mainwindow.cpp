#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "core/server.h"
#include "common.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->leIPAddress->setValidator( Chat::makeIPValidator(ui->leIPAddress) );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbStartServer_clicked()
{
    auto validator = ui->leIPAddress->validator();
    auto ipString = ui->leIPAddress->text();
    auto port = ui->sbPort->value();

    if(!port) {
        port = Chat::DEFAULT_PORT;
        QMessageBox::warning(this, tr("Warning"), tr("Port is 0. Will be using default port"));
        ui->sbPort->setValue(port);
    }

    if(checkConnectInfo(ipString, validator))
    {
        if(ipString == "localhost")
            ipString = "127.0.0.1";
        if(Server::getInstance().listen(QHostAddress(ipString), port))
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

bool MainWindow::checkConnectInfo(QString &ipString, const QValidator *validator)
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
    return true;
}
