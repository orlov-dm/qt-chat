#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractSocket>

class QTcpSocket;
class UsersModel;

template <class Key, class T>
class QMap;
class QCloseEvent;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QTcpSocket * socket, const QString& username, QWidget *parent = nullptr);
    ~MainWindow();

    void setSocketErrors(QMap<QAbstractSocket::SocketError, QString>* socketErrors) { _socketErrors = socketErrors; }
    void addUnhandledMessage(const QString &message);
protected:
    void closeEvent(QCloseEvent *event);
private slots:
    void onSocketError(QAbstractSocket::SocketError);
    void onSocketDisconnected();
    void onSocketReadyRead();
    void on_pbSend_clicked();

    void on_leMessage_returnPressed();

private:
    Ui::MainWindow *ui;

    UsersModel *_usersModel = nullptr;
    QTcpSocket *_socket = nullptr;
    QString _username;
    const QMap<QAbstractSocket::SocketError, QString> *_socketErrors = nullptr;

    void sendMessage(const QString &message);
    void handleRequest(const QString&);
};

#endif // MAINWINDOW_H
