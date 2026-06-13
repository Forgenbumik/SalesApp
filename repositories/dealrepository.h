#ifndef DEALREPOSITORY_H
#define DEALREPOSITORY_H

#include <QVector>
#include <QString>

#include "../models/deal.h"

class DealRepository
{
public:
    DealRepository();

    QVector<Deal> getAll() const;
    bool add(const Deal& deal);
    bool update(const Deal& deal);
    bool remove(int id);

private:
    int findCustomerIdByName(const QString& customerName) const;
    int findProductIdByName(const QString& productName) const;
};

#endif // DEALREPOSITORY_H
