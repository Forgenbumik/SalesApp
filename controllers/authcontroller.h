#ifndef AUTHCONTROLLER_H
#define AUTHCONTROLLER_H

#include <QObject>
#include <QString>

#include "../models/user.h"
#include "../repositories/userrepository.h"

class AuthController : public QObject
{
    Q_OBJECT

public:
    explicit AuthController(QObject* parent = nullptr);

    bool login(const QString& login, const QString& password, User& user);

    bool registerUser(const QString& login,
                      const QString& password,
                      const QString& repeatPassword,
                      const QString& fullName,
                      const QString& phone,
                      const QString& email,
                      QString& errorMessage);

    bool isPasswordStrong(const QString& password) const;

private:
    static UserRepository repository;

    bool isValidEmail(const QString& email) const;
    bool isValidPhone(const QString& phone) const;
};

#endif // AUTHCONTROLLER_H
