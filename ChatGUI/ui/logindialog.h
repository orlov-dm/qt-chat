#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QAbstractSocket>

class QTcpSocket;
class QValidator;

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

    void onSocketConnected();
    void onSocketError(QAbstractSocket::SocketError);
    void onSocketDisconnected();
    void onSocketReadyRead();
private:
    Ui::LoginDialog *ui;

    QTcpSocket *_socket;

    void sendMessage(const QString &message);
    bool checkConnectInfo(QString &ipString, const QValidator *validator, const QString &username);
    void startChatWindow(const QString &line);

    const int DEFAULT_PORT = 10000;

    QMap<QAbstractSocket::SocketError, QString> _socketErrors;
};

#endif // LOGINDIALOG_H
