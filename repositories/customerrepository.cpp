#include "customerrepository.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

CustomerRepository::CustomerRepository()
{
}

QVector<Customer> CustomerRepository::getAll() const
{
    QVector<Customer> customers;

    QSqlDatabase db = QSqlDatabase::database("main_connection");

    if (!db.isOpen()) {
        qDebug() << "Database is not open";
        return customers;
    }

    QSqlQuery query(db);

    query.prepare(
        "SELECT id, full_name, phone, email "
        "FROM customers "
        "ORDER BY id"
        );

    if (!query.exec()) {
        qDebug() << "getAll customers error:" << query.lastError().text();
        return customers;
    }

    while (query.next()) {
        customers.append(Customer(
            query.value("id").toInt(),
            query.value("full_name").toString(),
            query.value("phone").toString(),
            query.value("email").toString()
            ));
    }

    return customers;
}

bool CustomerRepository::add(const Customer& customer)
{
    QSqlDatabase db = QSqlDatabase::database("main_connection");

    if (!db.isOpen()) {
        qDebug() << "Database is not open";
        return false;
    }

    if (phoneExists(customer.phone())) {
        qDebug() << "Customer phone already exists";
        return false;
    }

    if (emailExists(customer.email())) {
        qDebug() << "Customer email already exists";
        return false;
    }

    QSqlQuery query(db);

    query.prepare(
        "INSERT INTO customers (full_name, phone, email) "
        "VALUES (:full_name, :phone, :email)"
        );

    query.bindValue(":full_name", customer.fullName());
    query.bindValue(":phone", customer.phone());
    query.bindValue(":email", customer.email());

    if (!query.exec()) {
        qDebug() << "add customer error:" << query.lastError().text();
        return false;
    }

    return true;
}

bool CustomerRepository::update(const Customer& customer)
{
    QSqlDatabase db = QSqlDatabase::database("main_connection");

    if (!db.isOpen()) {
        qDebug() << "Database is not open";
        return false;
    }

    if (phoneExists(customer.phone(), customer.id())) {
        qDebug() << "Customer phone already exists";
        return false;
    }

    if (emailExists(customer.email(), customer.id())) {
        qDebug() << "Customer email already exists";
        return false;
    }

    QSqlQuery query(db);

    query.prepare(
        "UPDATE customers "
        "SET full_name = :full_name, "
        "phone = :phone, "
        "email = :email "
        "WHERE id = :id"
        );

    query.bindValue(":full_name", customer.fullName());
    query.bindValue(":phone", customer.phone());
    query.bindValue(":email", customer.email());
    query.bindValue(":id", customer.id());

    if (!query.exec()) {
        qDebug() << "update customer error:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool CustomerRepository::remove(int id)
{
    QSqlDatabase db = QSqlDatabase::database("main_connection");

    if (!db.isOpen()) {
        qDebug() << "Database is not open";
        return false;
    }

    QSqlQuery query(db);

    query.prepare(
        "DELETE FROM customers "
        "WHERE id = :id"
        );

    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "remove customer error:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool CustomerRepository::phoneExists(const QString& phone, int excludeId) const
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
            "FROM customers "
            "WHERE phone = :phone AND id <> :id"
            );

        query.bindValue(":phone", phone);
        query.bindValue(":id", excludeId);
    } else {
        query.prepare(
            "SELECT COUNT(*) "
            "FROM customers "
            "WHERE phone = :phone"
            );

        query.bindValue(":phone", phone);
    }

    if (!query.exec()) {
        qDebug() << "phoneExists error:" << query.lastError().text();
        return false;
    }

    query.next();
    return query.value(0).toInt() > 0;
}

bool CustomerRepository::emailExists(const QString& email, int excludeId) const
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
            "FROM customers "
            "WHERE email = :email AND id <> :id"
            );

        query.bindValue(":email", email);
        query.bindValue(":id", excludeId);
    } else {
        query.prepare(
            "SELECT COUNT(*) "
            "FROM customers "
            "WHERE email = :email"
            );

        query.bindValue(":email", email);
    }

    if (!query.exec()) {
        qDebug() << "emailExists error:" << query.lastError().text();
        return false;
    }

    query.next();
    return query.value(0).toInt() > 0;
}
