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
    bool findById(int id, User& user) const;

    bool loginExists(const QString& login) const;
    bool emailExists(const QString& email) const;

    bool add(const User& user);
    bool updateProfile(const User& user);

private:
    int getUserRoleId() const;
};

#endif // USERREPOSITORY_H
