#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include <QVector>
#include "../models/user.h"

class UserRepository
{
public:
    UserRepository();

    QVector<User> getAll() const;
    bool findByLoginAndPassword(const QString& login, const QString& password, User& user) const;
    bool loginExists(const QString& login) const;
    void add(const User& user);

private:
    QVector<User> users;
    int nextId;
};

#endif // USERREPOSITORY_H
