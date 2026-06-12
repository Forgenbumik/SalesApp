#ifndef CUSTOMERCONTROLLER_H
#define CUSTOMERCONTROLLER_H

#include <QObject>
#include <QVector>
#include "../models/customer.h"
#include "../repositories/customerrepository.h"

class CustomerController : public QObject
{
    Q_OBJECT

public:
    explicit CustomerController(QObject* parent = nullptr);

    QVector<Customer> getCustomers() const;

    bool addCustomer(const QString& fullName, const QString& phone, const QString& email);
    bool updateCustomer(int id, const QString& fullName, const QString& phone, const QString& email);
    bool deleteCustomer(int id);

private:
    CustomerRepository repository;

    bool isValidCustomerData(const QString& fullName, const QString& phone, const QString& email) const;
};

#endif // CUSTOMERCONTROLLER_H
