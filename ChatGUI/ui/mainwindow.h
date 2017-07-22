#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTcpSocket;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QTcpSocket * serverConnection, QWidget *parent = nullptr);
    ~MainWindow();

    void addUnhandledMessage(const QString &message);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
