#ifndef PRODUCTREPOSITORY_H
#define PRODUCTREPOSITORY_H

#include <QVector>
#include "../models/product.h"

class ProductRepository
{
public:
    ProductRepository();

    QVector<Product> getAll() const;
    void add(const Product& product);
    bool update(const Product& product);
    bool remove(int id);

private:
    QVector<Product> products;
    int nextId;
};

#endif // PRODUCTREPOSITORY_H
