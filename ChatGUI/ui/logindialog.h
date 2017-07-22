#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QAbstractSocket>

class QTcpSocket;
class QValidator;
class MainWindow;

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

private slots:
    //auto-generated slots
    void on_pbConnect_clicked();
    void on_pbStop_clicked();

    void onSocketConnected();
    void onSocketError(QAbstractSocket::SocketError);
    void onSocketDisconnected();
    void onSocketReadyRead();

private:
    Ui::LoginDialog *ui;

    QTcpSocket *_socket = nullptr;
    MainWindow *_chatWindow = nullptr;

    void sendMessage(const QString &message);
    bool checkConnectInfo(QString &ipString, const QValidator *validator, const QString &username);
    void startChatWindow(const QString &line);
    void refreshInfo(bool isConnecting);

    const int DEFAULT_PORT = 10000;

    QMap<QAbstractSocket::SocketError, QString> _socketErrors;
    void initSocketErrors();


    const QString REG_SERVER_ADDR = "server_addr";
    const QString REG_SERVER_PORT = "server_port";
    const QString REG_USERNAME = "username";
    void saveData();
    void restoreData();
};

#endif // LOGINDIALOG_H
