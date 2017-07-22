#ifndef USERSMODEL_H
#define USERSMODEL_H
#include <QAbstractListModel>
#include <QList>
#include <QPair>
#include <QVariant>
#include <QHash>


class UsersModel: public QAbstractListModel {

    Q_OBJECT

public:
    explicit UsersModel(const QString &currentUsername, QObject* parent);
    ~UsersModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void init(const QStringList &users);
    void addUser(const QString& user, bool massive = false);
    void removeUser(const QString& user);
private:
    QList<QPair<int, QString>> _users;
    QHash<QString, int> _usersByName;
    QString _username;
};

#endif //USERSMODEL_H
