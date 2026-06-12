#include "authcontroller.h"

#include <QRegularExpression>

UserRepository AuthController::repository;

AuthController::AuthController(QObject* parent)
    : QObject(parent)
{
}

bool AuthController::login(const QString& login, const QString& password, User& user)
{
    return repository.findByLoginAndPassword(login.trimmed(), password, user);
}

bool AuthController::registerUser(const QString& login,
                                  const QString& password,
                                  const QString& repeatPassword,
                                  const QString& fullName,
                                  const QString& phone,
                                  const QString& email,
                                  QString& errorMessage)
{
    QString cleanLogin = login.trimmed();
    QString cleanFullName = fullName.trimmed();
    QString cleanPhone = phone.trimmed();
    QString cleanEmail = email.trimmed();

    if (cleanLogin.isEmpty()) {
        errorMessage = "Введите логин";
        return false;
    }

    if (cleanLogin.length() < 3) {
        errorMessage = "Логин должен быть не короче 3 символов";
        return false;
    }

    if (repository.loginExists(cleanLogin)) {
        errorMessage = "Пользователь с таким логином уже существует";
        return false;
    }

    if (cleanFullName.isEmpty()) {
        errorMessage = "Введите ФИО";
        return false;
    }

    if (!isValidPhone(cleanPhone)) {
        errorMessage = "Введите телефон в формате +79991234567";
        return false;
    }

    if (!isValidEmail(cleanEmail)) {
        errorMessage = "Введите корректный email";
        return false;
    }

    if (password != repeatPassword) {
        errorMessage = "Пароли не совпадают";
        return false;
    }

    if (!isPasswordStrong(password)) {
        errorMessage = "Пароль: минимум 8 символов, цифра, заглавная буква и спецсимвол";
        return false;
    }

    User user(
        -1,
        cleanLogin,
        password,
        cleanFullName,
        cleanPhone,
        cleanEmail,
        "user"
        );

    repository.add(user);

    errorMessage = "";
    return true;
}

bool AuthController::isPasswordStrong(const QString& password) const
{
    QRegularExpression regex("^(?=.*[A-Z])(?=.*\\d)(?=.*[^A-Za-z0-9]).{8,}$");
    return regex.match(password).hasMatch();
}

bool AuthController::isValidEmail(const QString& email) const
{
    QRegularExpression regex("^[\\w\\.\\-]+@[\\w\\.-]+\\.[A-Za-z]{2,}$");
    return regex.match(email).hasMatch();
}

bool AuthController::isValidPhone(const QString& phone) const
{
    QRegularExpression regex("^\\+?[0-9]{10,15}$");
    return regex.match(phone).hasMatch();
}
