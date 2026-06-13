#include "dealrepository.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

DealRepository::DealRepository()
{
}

QVector<Deal> DealRepository::getAll() const
{
    QVector<Deal> deals;

    QSqlDatabase db = QSqlDatabase::database("main_connection");

    if (!db.isOpen()) {
        qDebug() << "Database is not open";
        return deals;
    }

    QSqlQuery query(db);

    query.prepare(
        "SELECT "
        "d.id, "
        "c.full_name AS customer_name, "
        "p.name AS product_name, "
        "d.quantity, "
        "d.total_amount, "
        "d.status "
        "FROM deals d "
        "JOIN customers c ON c.id = d.customer_id "
        "JOIN products p ON p.id = d.product_id "
        "ORDER BY d.id"
        );

    if (!query.exec()) {
        qDebug() << "getAll deals error:" << query.lastError().text();
        return deals;
    }

    while (query.next()) {
        deals.append(Deal(
            query.value("id").toInt(),
            query.value("customer_name").toString(),
            query.value("product_name").toString(),
            query.value("quantity").toInt(),
            query.value("total_amount").toDouble(),
            query.value("status").toString()
            ));
    }

    return deals;
}

bool DealRepository::add(const Deal& deal)
{
    QSqlDatabase db = QSqlDatabase::database("main_connection");

    if (!db.isOpen()) {
        qDebug() << "Database is not open";
        return false;
    }

    int customerId = findCustomerIdByName(deal.customerName());
    int productId = findProductIdByName(deal.productName());

    if (customerId <= 0) {
        qDebug() << "Customer not found:" << deal.customerName();
        return false;
    }

    if (productId <= 0) {
        qDebug() << "Product not found:" << deal.productName();
        return false;
    }

    QSqlQuery query(db);

    query.prepare(
        "INSERT INTO deals (customer_id, product_id, quantity, total_amount, status) "
        "VALUES (:customer_id, :product_id, :quantity, :total_amount, :status)"
        );

    query.bindValue(":customer_id", customerId);
    query.bindValue(":product_id", productId);
    query.bindValue(":quantity", deal.quantity());
    query.bindValue(":total_amount", deal.totalAmount());
    query.bindValue(":status", deal.status());

    if (!query.exec()) {
        qDebug() << "add deal error:" << query.lastError().text();
        return false;
    }

    return true;
}

bool DealRepository::update(const Deal& deal)
{
    QSqlDatabase db = QSqlDatabase::database("main_connection");

    if (!db.isOpen()) {
        qDebug() << "Database is not open";
        return false;
    }

    int customerId = findCustomerIdByName(deal.customerName());
    int productId = findProductIdByName(deal.productName());

    if (customerId <= 0) {
        qDebug() << "Customer not found:" << deal.customerName();
        return false;
    }

    if (productId <= 0) {
        qDebug() << "Product not found:" << deal.productName();
        return false;
    }

    QSqlQuery query(db);

    query.prepare(
        "UPDATE deals "
        "SET customer_id = :customer_id, "
        "product_id = :product_id, "
        "quantity = :quantity, "
        "total_amount = :total_amount, "
        "status = :status "
        "WHERE id = :id"
        );

    query.bindValue(":customer_id", customerId);
    query.bindValue(":product_id", productId);
    query.bindValue(":quantity", deal.quantity());
    query.bindValue(":total_amount", deal.totalAmount());
    query.bindValue(":status", deal.status());
    query.bindValue(":id", deal.id());

    if (!query.exec()) {
        qDebug() << "update deal error:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool DealRepository::remove(int id)
{
    QSqlDatabase db = QSqlDatabase::database("main_connection");

    if (!db.isOpen()) {
        qDebug() << "Database is not open";
        return false;
    }

    QSqlQuery query(db);

    query.prepare(
        "DELETE FROM deals "
        "WHERE id = :id"
        );

    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "remove deal error:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

int DealRepository::findCustomerIdByName(const QString& customerName) const
{
    QSqlDatabase db = QSqlDatabase::database("main_connection");

    if (!db.isOpen()) {
        qDebug() << "Database is not open";
        return -1;
    }

    QSqlQuery query(db);

    query.prepare(
        "SELECT id "
        "FROM customers "
        "WHERE full_name = :full_name "
        "ORDER BY id "
        "LIMIT 1"
        );

    query.bindValue(":full_name", customerName.trimmed());

    if (!query.exec()) {
        qDebug() << "findCustomerIdByName error:" << query.lastError().text();
        return -1;
    }

    if (!query.next()) {
        return -1;
    }

    return query.value("id").toInt();
}

int DealRepository::findProductIdByName(const QString& productName) const
{
    QSqlDatabase db = QSqlDatabase::database("main_connection");

    if (!db.isOpen()) {
        qDebug() << "Database is not open";
        return -1;
    }

    QSqlQuery query(db);

    query.prepare(
        "SELECT id "
        "FROM products "
        "WHERE name = :name "
        "ORDER BY id "
        "LIMIT 1"
        );

    query.bindValue(":name", productName.trimmed());

    if (!query.exec()) {
        qDebug() << "findProductIdByName error:" << query.lastError().text();
        return -1;
    }

    if (!query.next()) {
        return -1;
    }

    return query.value("id").toInt();
}
