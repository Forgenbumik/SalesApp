#ifndef USER_H
#define USER_H

#include <QString>

class User
{
public:
    User();

    User(int id,
         const QString& login,
         const QString& password,
         const QString& fullName,
         const QString& phone,
         const QString& email,
         const QString& role);

    int id() const;
    QString login() const;
    QString password() const;
    QString fullName() const;
    QString phone() const;
    QString email() const;
    QString role() const;

    void setId(int id);
    void setLogin(const QString& login);
    void setPassword(const QString& password);
    void setFullName(const QString& fullName);
    void setPhone(const QString& phone);
    void setEmail(const QString& email);
    void setRole(const QString& role);

    bool isAdmin() const;

private:
    int m_id;
    QString m_login;
    QString m_password;
    QString m_fullName;
    QString m_phone;
    QString m_email;
    QString m_role;
};

#endif // USER_H
