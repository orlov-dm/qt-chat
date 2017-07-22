#include "usersmodel.h"
#include <QDebug>
#include <QFont>
#include <cassert>


UsersModel::UsersModel(const QString &currentUsername, QObject *parent):
    QAbstractListModel(parent),
    _username(currentUsername)
{

}

UsersModel::~UsersModel()
{

}

int UsersModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : _users.count();
}

QVariant UsersModel::data(const QModelIndex &index, int role) const
{
    QVariant result;
    if(!index.isValid() || index.row() >= _users.size() ) {
        return result;
    }

    switch(role)
    {
        case Qt::DisplayRole:
            result = _users[index.row()].second;
        break;
        case Qt::FontRole:
        {
            auto username = _users[index.row()].second;
            if(username == _username)
            {
                QFont boldFont;
                boldFont.setBold(true);
                result = boldFont;
            }
            break;
        }
        default:
        break;
    }

    return result;
}

void UsersModel::init(const QStringList &users)
{
    _users.clear();
    _usersByName.clear();
    _users.reserve(users.count());
    beginInsertRows(QModelIndex(), 0, users.count() - 1);
    for(auto user: users)
    {
        addUser(user, true);
    }
    endInsertRows();
}

void UsersModel::addUser(const QString &user, bool massive)
{
    auto newRowIndex = _users.count();
    if(!massive)
        beginInsertRows(QModelIndex(), newRowIndex, newRowIndex);
    _users.append({newRowIndex, user});
    _usersByName[user] = newRowIndex;
    if(!massive)
        endInsertRows();
}

void UsersModel::removeUser(const QString &user)
{
    if(!_usersByName.contains(user))
        return;

    auto rowIndex = _usersByName[user];

    assert(rowIndex < _users.count());

    beginRemoveRows(QModelIndex(), rowIndex, rowIndex);
    _usersByName.remove(user);
    _users.removeAt(rowIndex);
    endRemoveRows();
}


