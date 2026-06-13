#include "productcontroller.h"

ProductController::ProductController(QObject* parent)
    : QObject(parent)
{
}

QVector<Product> ProductController::getProducts() const
{
    return repository.getAll();
}

bool ProductController::addProduct(const QString& name, double price, int quantity)
{
    if (!isValidProductData(name, price, quantity)) {
        return false;
    }

    Product product(-1, name.trimmed(), price, quantity);
    return repository.add(product);
}

bool ProductController::updateProduct(int id, const QString& name, double price, int quantity)
{
    if (id <= 0 || !isValidProductData(name, price, quantity)) {
        return false;
    }

    Product product(id, name.trimmed(), price, quantity);
    return repository.update(product);
}

bool ProductController::deleteProduct(int id)
{
    if (id <= 0) {
        return false;
    }

    return repository.remove(id);
}

bool ProductController::isValidProductData(const QString& name, double price, int quantity) const
{
    if (name.trimmed().isEmpty()) {
        return false;
    }

    if (price < 0) {
        return false;
    }

    if (quantity < 0) {
        return false;
    }

    return true;
}
