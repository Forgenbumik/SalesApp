#ifndef DEALREPOSITORY_H
#define DEALREPOSITORY_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QVector>

#include "../models/deal.h"
#include "../models/user.h"

class DealRepository
{
public:
    DealRepository();

    QVector<Deal> getAll(
        const User& currentUser
        ) const;

    bool findById(
        int dealId,
        const User& currentUser,
        Deal& deal
        ) const;

    bool add(
        Deal& deal,
        const User& currentUser
        );

    bool update(
        Deal& deal,
        const User& currentUser
        );

    bool remove(
        int dealId,
        const User& currentUser
        );

    QString lastError() const;

private:
    mutable QString m_lastError;

    bool loadItems(
        int dealId,
        const QSqlDatabase& db,
        QVector<DealItem>& items
        ) const;

    bool findCustomerName(
        int customerId,
        const QSqlDatabase& db,
        QString& customerName
        );

    bool prepareItems(
        const QVector<DealItem>& requestedItems,
        bool reserveStock,
        const QSqlDatabase& db,
        QVector<DealItem>& preparedItems,
        double& subtotal
        );

    bool resolveDiscount(
        int discountRuleId,
        double subtotal,
        const QSqlDatabase& db,
        QString& discountRuleName,
        double& discountPercent
        );

    bool insertItems(
        int dealId,
        const QSqlDatabase& db,
        QVector<DealItem>& items
        );

    bool restoreStock(
        const QVector<DealItem>& items,
        const QSqlDatabase& db
        );

    bool setError(
        const QString& message
        ) const;

    bool setQueryError(
        const QString& context,
        const QSqlQuery& query
        ) const;

    static bool usesStock(
        const QString& status
        );

    static double roundMoney(
        double value
        );
};

#endif // DEALREPOSITORY_H
