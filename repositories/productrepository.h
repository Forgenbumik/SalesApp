#ifndef PRODUCTREPOSITORY_H
#define PRODUCTREPOSITORY_H

#include <QVector>
#include "../models/product.h"

class ProductRepository
{
public:
    ProductRepository();

    QVector<Product> getAll() const;
    bool add(const Product& product);
    bool update(const Product& product);
    bool remove(int id);

private:
    bool productNameExists(const QString& name, int excludeId = -1) const;
};

#endif // PRODUCTREPOSITORY_H
