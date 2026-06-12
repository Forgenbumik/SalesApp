#include "customer.h"

Customer::Customer()
    : m_id(-1), m_fullName(""), m_phone(""), m_email("")
{
}

Customer::Customer(int id, const QString& fullName, const QString& phone, const QString& email)
    : m_id(id), m_fullName(fullName), m_phone(phone), m_email(email)
{
}

int Customer::id() const
{
    return m_id;
}

QString Customer::fullName() const
{
    return m_fullName;
}

QString Customer::phone() const
{
    return m_phone;
}

QString Customer::email() const
{
    return m_email;
}

void Customer::setId(int id)
{
    m_id = id;
}

void Customer::setFullName(const QString& fullName)
{
    m_fullName = fullName;
}

void Customer::setPhone(const QString& phone)
{
    m_phone = phone;
}

void Customer::setEmail(const QString& email)
{
    m_email = email;
}
