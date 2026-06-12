#include "authcontroller.h"

#include <QRegularExpression>

AuthController::AuthController(QObject* parent)
    : QObject(parent)
{
}

bool AuthController::login(const QString& login, const QString& password, User& user)
{
    // Временная авторизация без базы данных.
    // Потом заменим это на запрос к PostgreSQL.

    if (login == "admin" && password == "Admin123!") {
        user = User(1, "admin", "admin");
        return true;
    }

    if (login == "user" && password == "User123!") {
        user = User(2, "user", "user");
        return true;
    }

    return false;
}

bool AuthController::isPasswordStrong(const QString& password) const
{
    // Минимум 8 символов, одна цифра, одна заглавная буква, один спецсимвол
    QRegularExpression regex("^(?=.*[A-Z])(?=.*\\d)(?=.*[^A-Za-z0-9]).{8,}$");
    return regex.match(password).hasMatch();
}
