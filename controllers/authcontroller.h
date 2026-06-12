#ifndef AUTHCONTROLLER_H
#define AUTHCONTROLLER_H

#include <QObject>
#include <QString>
#include "../models/User.h"

class AuthController : public QObject
{
    Q_OBJECT

public:
    explicit AuthController(QObject* parent = nullptr);

    bool login(const QString& login, const QString& password, User& user);
    bool isPasswordStrong(const QString& password) const;
};

#endif // AUTHCONTROLLER_H
