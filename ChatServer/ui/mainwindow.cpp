#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ipRegex (QString("^%1\\.%2\\.%3\\.%4$").arg(ipRange, ipRange, ipRange, ipRange));

    auto ipValidator = new QRegExpValidator(ipRegex, ui->leIPAddress);
    ui->leIPAddress->setValidator( ipValidator );
}

MainWindow::~MainWindow()
{
    delete ui;
}
