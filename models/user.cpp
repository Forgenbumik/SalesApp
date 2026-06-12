#include "user.h"

User::User()
    : m_id(-1),
    m_login(""),
    m_password(""),
    m_fullName(""),
    m_phone(""),
    m_email(""),
    m_role("")
{
}

User::User(int id,
           const QString& login,
           const QString& password,
           const QString& fullName,
           const QString& phone,
           const QString& email,
           const QString& role)
    : m_id(id),
    m_login(login),
    m_password(password),
    m_fullName(fullName),
    m_phone(phone),
    m_email(email),
    m_role(role)
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

QString User::password() const
{
    return m_password;
}

QString User::fullName() const
{
    return m_fullName;
}

QString User::phone() const
{
    return m_phone;
}

QString User::email() const
{
    return m_email;
}

QString User::role() const
{
    return m_role;
}

void User::setId(int id)
{
    m_id = id;
}

void User::setLogin(const QString& login)
{
    m_login = login;
}

void User::setPassword(const QString& password)
{
    m_password = password;
}

void User::setFullName(const QString& fullName)
{
    m_fullName = fullName;
}

void User::setPhone(const QString& phone)
{
    m_phone = phone;
}

void User::setEmail(const QString& email)
{
    m_email = email;
}

void User::setRole(const QString& role)
{
    m_role = role;
}

bool User::isAdmin() const
{
    return m_role == "admin";
}
