#include "discountrulerepository.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

DiscountRuleRepository::DiscountRuleRepository()
{
}

QVector<DiscountRule> DiscountRuleRepository::getAll() const
{
    QVector<DiscountRule> rules;

    QSqlDatabase db = QSqlDatabase::database("main_connection");

    if (!db.isOpen()) {
        qDebug() << "Database is not open";
        return rules;
    }

    QSqlQuery query(db);

    query.prepare(
        "SELECT id, name, min_amount, discount_percent "
        "FROM discount_rules "
        "ORDER BY id"
        );

    if (!query.exec()) {
        qDebug() << "getAll discount rules error:" << query.lastError().text();
        return rules;
    }

    while (query.next()) {
        rules.append(DiscountRule(
            query.value("id").toInt(),
            query.value("name").toString(),
            query.value("min_amount").toDouble(),
            query.value("discount_percent").toDouble()
            ));
    }

    return rules;
}

bool DiscountRuleRepository::add(const DiscountRule& rule)
{
    QSqlDatabase db = QSqlDatabase::database("main_connection");

    if (!db.isOpen()) {
        qDebug() << "Database is not open";
        return false;
    }

    if (ruleNameExists(rule.name())) {
        qDebug() << "Discount rule name already exists";
        return false;
    }

    QSqlQuery query(db);

    query.prepare(
        "INSERT INTO discount_rules (name, min_amount, discount_percent) "
        "VALUES (:name, :min_amount, :discount_percent)"
        );

    query.bindValue(":name", rule.name());
    query.bindValue(":min_amount", rule.minAmount());
    query.bindValue(":discount_percent", rule.discountPercent());

    if (!query.exec()) {
        qDebug() << "add discount rule error:" << query.lastError().text();
        return false;
    }

    return true;
}

bool DiscountRuleRepository::update(const DiscountRule& rule)
{
    QSqlDatabase db = QSqlDatabase::database("main_connection");

    if (!db.isOpen()) {
        qDebug() << "Database is not open";
        return false;
    }

    if (ruleNameExists(rule.name(), rule.id())) {
        qDebug() << "Discount rule name already exists";
        return false;
    }

    QSqlQuery query(db);

    query.prepare(
        "UPDATE discount_rules "
        "SET name = :name, "
        "min_amount = :min_amount, "
        "discount_percent = :discount_percent "
        "WHERE id = :id"
        );

    query.bindValue(":name", rule.name());
    query.bindValue(":min_amount", rule.minAmount());
    query.bindValue(":discount_percent", rule.discountPercent());
    query.bindValue(":id", rule.id());

    if (!query.exec()) {
        qDebug() << "update discount rule error:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool DiscountRuleRepository::remove(int id)
{
    QSqlDatabase db = QSqlDatabase::database("main_connection");

    if (!db.isOpen()) {
        qDebug() << "Database is not open";
        return false;
    }

    QSqlQuery query(db);

    query.prepare(
        "DELETE FROM discount_rules "
        "WHERE id = :id"
        );

    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "remove discount rule error:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool DiscountRuleRepository::ruleNameExists(const QString& name, int excludeId) const
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
            "FROM discount_rules "
            "WHERE name = :name AND id <> :id"
            );

        query.bindValue(":name", name);
        query.bindValue(":id", excludeId);
    } else {
        query.prepare(
            "SELECT COUNT(*) "
            "FROM discount_rules "
            "WHERE name = :name"
            );

        query.bindValue(":name", name);
    }

    if (!query.exec()) {
        qDebug() << "ruleNameExists error:" << query.lastError().text();
        return false;
    }

    query.next();
    return query.value(0).toInt() > 0;
}
