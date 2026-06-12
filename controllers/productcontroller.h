#ifndef PRODUCTCONTROLLER_H
#define PRODUCTCONTROLLER_H

#include <QObject>
#include <QVector>
#include "../models/product.h"
#include "../repositories/productrepository.h"

class ProductController : public QObject
{
    Q_OBJECT

public:
    explicit ProductController(QObject* parent = nullptr);

    QVector<Product> getProducts() const;

    bool addProduct(const QString& name, double price, int quantity);
    bool updateProduct(int id, const QString& name, double price, int quantity);
    bool deleteProduct(int id);

private:
    ProductRepository repository;

    bool isValidProductData(const QString& name, double price, int quantity) const;
};
#endif // PRODUCTCONTROLLER_H
