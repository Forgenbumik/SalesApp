#include "productrepository.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

ProductRepository::ProductRepository()
{
}

QVector<Product> ProductRepository::getAll() const
{
    QVector<Product> products;

    QSqlDatabase db = QSqlDatabase::database("main_connection");

    if (!db.isOpen()) {
        qDebug() << "Database is not open";
        return products;
    }

    QSqlQuery query(db);

    query.prepare(
        "SELECT id, name, price, quantity "
        "FROM products "
        "ORDER BY id"
        );

    if (!query.exec()) {
        qDebug() << "getAll products error:" << query.lastError().text();
        return products;
    }

    while (query.next()) {
        products.append(Product(
            query.value("id").toInt(),
            query.value("name").toString(),
            query.value("price").toDouble(),
            query.value("quantity").toInt()
            ));
    }

    return products;
}

bool ProductRepository::add(const Product& product)
{
    QSqlDatabase db = QSqlDatabase::database("main_connection");

    if (!db.isOpen()) {
        qDebug() << "Database is not open";
        return false;
    }

    if (productNameExists(product.name())) {
        qDebug() << "Product name already exists";
        return false;
    }

    QSqlQuery query(db);

    query.prepare(
        "INSERT INTO products (name, price, quantity) "
        "VALUES (:name, :price, :quantity)"
        );

    query.bindValue(":name", product.name());
    query.bindValue(":price", product.price());
    query.bindValue(":quantity", product.quantity());

    if (!query.exec()) {
        qDebug() << "add product error:" << query.lastError().text();
        return false;
    }

    return true;
}

bool ProductRepository::update(const Product& product)
{
    QSqlDatabase db = QSqlDatabase::database("main_connection");

    if (!db.isOpen()) {
        qDebug() << "Database is not open";
        return false;
    }

    if (productNameExists(product.name(), product.id())) {
        qDebug() << "Product name already exists";
        return false;
    }

    QSqlQuery query(db);

    query.prepare(
        "UPDATE products "
        "SET name = :name, "
        "price = :price, "
        "quantity = :quantity "
        "WHERE id = :id"
        );

    query.bindValue(":name", product.name());
    query.bindValue(":price", product.price());
    query.bindValue(":quantity", product.quantity());
    query.bindValue(":id", product.id());

    if (!query.exec()) {
        qDebug() << "update product error:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool ProductRepository::remove(int id)
{
    QSqlDatabase db = QSqlDatabase::database("main_connection");

    if (!db.isOpen()) {
        qDebug() << "Database is not open";
        return false;
    }

    QSqlQuery query(db);

    query.prepare(
        "DELETE FROM products "
        "WHERE id = :id"
        );

    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "remove product error:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool ProductRepository::productNameExists(const QString& name, int excludeId) const
{
    QSqlDatabase db = QSqlDatabase::database("main_connection");

    if (!db.isOpen()) {
        qDebug() << "Database is not open";
        return false;
    }

    QSqlQuery query(db);

    if (excludeId > 0) {
        query.prepare(
            "SELECT COUNT(*) "
            "FROM products "
            "WHERE name = :name AND id <> :id"
            );

        query.bindValue(":name", name);
        query.bindValue(":id", excludeId);
    } else {
        query.prepare(
            "SELECT COUNT(*) "
            "FROM products "
            "WHERE name = :name"
            );

        query.bindValue(":name", name);
    }

    if (!query.exec()) {
        qDebug() << "productNameExists error:" << query.lastError().text();
        return false;
    }

    query.next();
    return query.value(0).toInt() > 0;
}
