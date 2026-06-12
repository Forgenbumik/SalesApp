#include "productrepository.h"

ProductRepository::ProductRepository()
    : nextId(4)
{
    products.append(Product(1, "Ноутбук", 75000.0, 10));
    products.append(Product(2, "Смартфон", 45000.0, 25));
    products.append(Product(3, "Наушники", 8000.0, 40));
}

QVector<Product> ProductRepository::getAll() const
{
    return products;
}

void ProductRepository::add(const Product& product)
{
    Product newProduct = product;
    newProduct.setId(nextId++);
    products.append(newProduct);
}

bool ProductRepository::update(const Product& product)
{
    for (Product& item : products) {
        if (item.id() == product.id()) {
            item = product;
            return true;
        }
    }

    return false;
}

bool ProductRepository::remove(int id)
{
    for (int i = 0; i < products.size(); ++i) {
        if (products[i].id() == id) {
            products.removeAt(i);
            return true;
        }
    }

    return false;
}
