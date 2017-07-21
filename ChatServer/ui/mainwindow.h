#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Server;

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
};

#endif // MAINWINDOW_H
