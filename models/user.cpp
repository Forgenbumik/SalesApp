#include "user.h"

User::User()
    : m_id(-1), m_login(""), m_role("")
{
}

User::User(int id, const QString& login, const QString& role)
    : m_id(id), m_login(login), m_role(role)
{
}

int User::id() const
{
    return m_id;
}

QString User::login() const
{
    return m_login;
}

QString User::role() const
{
    return m_role;
}

bool User::isAdmin() const
{
    return m_role == "admin";
}
