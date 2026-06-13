#ifndef CUSTOMERREPOSITORY_H
#define CUSTOMERREPOSITORY_H

#include <QVector>
#include "../models/customer.h"

class CustomerRepository
{
public:
    CustomerRepository();

    QVector<Customer> getAll() const;
    bool add(const Customer& customer);
    bool update(const Customer& customer);
    bool remove(int id);

private:
    bool phoneExists(const QString& phone, int excludeId = -1) const;
    bool emailExists(const QString& email, int excludeId = -1) const;
};
#endif // CUSTOMERREPOSITORY_H
