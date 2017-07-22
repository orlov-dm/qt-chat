#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QTcpSocket *serverConnection, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addUnhandledMessage(const QString &message)
{

}
