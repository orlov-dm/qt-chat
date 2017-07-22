#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>


class Server;
class QValidator;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pbStartServer_clicked();

    void on_pbStopServer_clicked();

private:
    Ui::MainWindow *ui;
    bool checkConnectInfo(QString &ipString, const QValidator *validator);


    const QString REG_SERVER_ADDR = "server_addr";
    const QString REG_SERVER_PORT = "server_port";
    void saveData();
    void restoreData();
};

#endif // MAINWINDOW_H
