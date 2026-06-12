#ifndef DEALREPOSITORY_H
#define DEALREPOSITORY_H

#include <QVector>
#include "../models/deal.h"

class DealRepository
{
public:
    DealRepository();

    QVector<Deal> getAll() const;
    void add(const Deal& deal);
    bool update(const Deal& deal);
    bool remove(int id);

private:
    QVector<Deal> deals;
    int nextId;
};

#endif // DEALREPOSITORY_H
