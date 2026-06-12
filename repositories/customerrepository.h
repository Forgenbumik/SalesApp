#ifndef CUSTOMERREPOSITORY_H
#define CUSTOMERREPOSITORY_H

#include <QVector>
#include "../models/customer.h"

class CustomerRepository
{
public:
    CustomerRepository();

    QVector<Customer> getAll() const;
    void add(const Customer& customer);
    bool update(const Customer& customer);
    bool remove(int id);

private:
    QVector<Customer> customers;
    int nextId;
};

#endif // CUSTOMERREPOSITORY_H
