#include "userrepository.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

UserRepository::UserRepository()
{
}

QVector<User> UserRepository::getAll() const
{
    QVector<User> users;

    QSqlDatabase db = QSqlDatabase::database("main_connection");

    if (!db.isOpen()) {
        qDebug() << "Database is not open";
        return users;
    }

    QSqlQuery query(db);

    query.prepare(
        "SELECT u.id, u.login, u.password, u.full_name, u.phone, u.email, r.name AS role "
        "FROM users u "
        "JOIN roles r ON r.id = u.role_id "
        "ORDER BY u.id"
        );

    if (!query.exec()) {
        qDebug() << "getAll users error:" << query.lastError().text();
        return users;
    }

    while (query.next()) {
        users.append(User(
            query.value("id").toInt(),
            query.value("login").toString(),
            query.value("password").toString(),
            query.value("full_name").toString(),
            query.value("phone").toString(),
            query.value("email").toString(),
            query.value("role").toString()
            ));
    }

    return users;
}

bool UserRepository::findByLoginAndPassword(const QString& login, const QString& password, User& user) const
{
    QSqlDatabase db = QSqlDatabase::database("main_connection");

    if (!db.isOpen()) {
        qDebug() << "Database is not open";
        return false;
    }

    QSqlQuery query(db);

    query.prepare(
        "SELECT u.id, u.login, u.password, u.full_name, u.phone, u.email, r.name AS role "
        "FROM users u "
        "JOIN roles r ON r.id = u.role_id "
        "WHERE u.login = :login AND u.password = :password"
        );

    query.bindValue(":login", login);
    query.bindValue(":password", password);

    if (!query.exec()) {
        qDebug() << "findByLoginAndPassword error:" << query.lastError().text();
        return false;
    }

    if (!query.next()) {
        return false;
    }

    user = User(
        query.value("id").toInt(),
        query.value("login").toString(),
        query.value("password").toString(),
        query.value("full_name").toString(),
        query.value("phone").toString(),
        query.value("email").toString(),
        query.value("role").toString()
        );

    return true;
}

bool UserRepository::findById(int id, User& user) const
{
    QSqlDatabase db = QSqlDatabase::database("main_connection");

    if (!db.isOpen()) {
        qDebug() << "Database is not open";
        return false;
    }

    QSqlQuery query(db);

    query.prepare(
        "SELECT u.id, u.login, u.password, u.full_name, u.phone, u.email, r.name AS role "
        "FROM users u "
        "JOIN roles r ON r.id = u.role_id "
        "WHERE u.id = :id"
        );

    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "findById error:" << query.lastError().text();
        return false;
    }

    if (!query.next()) {
        return false;
    }

    user = User(
        query.value("id").toInt(),
        query.value("login").toString(),
        query.value("password").toString(),
        query.value("full_name").toString(),
        query.value("phone").toString(),
        query.value("email").toString(),
        query.value("role").toString()
        );

    return true;
}

bool UserRepository::loginExists(const QString& login) const
{
    QSqlDatabase db = QSqlDatabase::database("main_connection");

    if (!db.isOpen()) {
        qDebug() << "Database is not open";
        return false;
    }

    QSqlQuery query(db);

    query.prepare("SELECT COUNT(*) FROM users WHERE login = :login");
    query.bindValue(":login", login);

    if (!query.exec()) {
        qDebug() << "loginExists error:" << query.lastError().text();
        return false;
    }

    query.next();
    return query.value(0).toInt() > 0;
}

bool UserRepository::emailExists(const QString& email) const
{
    QSqlDatabase db = QSqlDatabase::database("main_connection");

    if (!db.isOpen()) {
        qDebug() << "Database is not open";
        return false;
    }

    QSqlQuery query(db);

    query.prepare("SELECT COUNT(*) FROM users WHERE email = :email");
    query.bindValue(":email", email);

    if (!query.exec()) {
        qDebug() << "emailExists error:" << query.lastError().text();
        return false;
    }

    query.next();
    return query.value(0).toInt() > 0;
}

bool UserRepository::add(const User& user)
{
    QSqlDatabase db = QSqlDatabase::database("main_connection");

    if (!db.isOpen()) {
        qDebug() << "Database is not open";
        return false;
    }

    int roleId = getUserRoleId();

    if (roleId <= 0) {
        qDebug() << "Role user not found";
        return false;
    }

    QSqlQuery query(db);

    query.prepare(
        "INSERT INTO users (login, password, full_name, phone, email, role_id) "
        "VALUES (:login, :password, :full_name, :phone, :email, :role_id)"
        );

    query.bindValue(":login", user.login());
    query.bindValue(":password", user.password());
    query.bindValue(":full_name", user.fullName());
    query.bindValue(":phone", user.phone());
    query.bindValue(":email", user.email());
    query.bindValue(":role_id", roleId);

    if (!query.exec()) {
        qDebug() << "add user error:" << query.lastError().text();
        return false;
    }

    return true;
}

bool UserRepository::updateProfile(const User& user)
{
    QSqlDatabase db = QSqlDatabase::database("main_connection");

    if (!db.isOpen()) {
        qDebug() << "Database is not open";
        return false;
    }

    QSqlQuery query(db);

    query.prepare(
        "UPDATE users "
        "SET full_name = :full_name, "
        "phone = :phone, "
        "email = :email, "
        "password = :password "
        "WHERE id = :id"
        );

    query.bindValue(":full_name", user.fullName());
    query.bindValue(":phone", user.phone());
    query.bindValue(":email", user.email());
    query.bindValue(":password", user.password());
    query.bindValue(":id", user.id());

    if (!query.exec()) {
        qDebug() << "updateProfile error:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

int UserRepository::getUserRoleId() const
{
    QSqlDatabase db = QSqlDatabase::database("main_connection");

    if (!db.isOpen()) {
        return -1;
    }

    QSqlQuery query(db);

    query.prepare("SELECT id FROM roles WHERE name = :name");
    query.bindValue(":name", "user");

    if (!query.exec()) {
        qDebug() << "getUserRoleId error:" << query.lastError().text();
        return -1;
    }

    if (!query.next()) {
        return -1;
    }

    return query.value("id").toInt();
}
