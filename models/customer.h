#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <QString>

class Customer
{
public:
    Customer();
    Customer(int id, const QString& fullName, const QString& phone, const QString& email);

    int id() const;
    QString fullName() const;
    QString phone() const;
    QString email() const;

    void setId(int id);
    void setFullName(const QString& fullName);
    void setPhone(const QString& phone);
    void setEmail(const QString& email);

private:
    int m_id;
    QString m_fullName;
    QString m_phone;
    QString m_email;
};

#endif // CUSTOMER_H
