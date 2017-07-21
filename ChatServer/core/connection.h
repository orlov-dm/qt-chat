#ifndef CONNECTION_H
#define CONNECTION_H
#include <QTcpSocket>

class Connection : public QTcpSocket
{
    Q_OBJECT

public:
    Connection(QObject *parent);

    void setClientName(const QString &name) { _clientName = name; }
    QString getClientName() const { return _clientName; }

private:
    QString _clientName;
};

#endif // CONNECTION_H
