#include "userrepository.h"

UserRepository::UserRepository()
    : nextId(3)
{
    users.append(User(
        1,
        "admin",
        "Admin123!",
        "Администратор системы",
        "+79990000001",
        "admin@example.com",
        "admin"
        ));

    users.append(User(
        2,
        "user",
        "User123!",
        "Обычный пользователь",
        "+79990000002",
        "user@example.com",
        "user"
        ));
}

QVector<User> UserRepository::getAll() const
{
    return users;
}

bool UserRepository::findByLoginAndPassword(const QString& login, const QString& password, User& user) const
{
    for (const User& item : users) {
        if (item.login() == login && item.password() == password) {
            user = item;
            return true;
        }
    }

    return false;
}

bool UserRepository::loginExists(const QString& login) const
{
    for (const User& item : users) {
        if (item.login() == login) {
            return true;
        }
    }

    return false;
}

void UserRepository::add(const User& user)
{
    User newUser = user;
    newUser.setId(nextId++);
    users.append(newUser);
}
