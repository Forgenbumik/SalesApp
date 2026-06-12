#ifndef USER_H
#define USER_H

#include <QString>

class User
{
public:
    User();
    User(int id, const QString& login, const QString& role);

    int id() const;
    QString login() const;
    QString role() const;

    bool isAdmin() const;

private:
    int m_id;
    QString m_login;
    QString m_role;
};

#endif // USER_H
