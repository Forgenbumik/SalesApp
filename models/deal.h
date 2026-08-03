#ifndef DEAL_H
#define DEAL_H

#include <QDateTime>
#include <QString>
#include <QVector>

#include "dealitem.h"

class Deal
{
public:
    Deal();

    // Создание новой сделки
    Deal(int customerId,
         const QString& customerName,
         int discountRuleId,
         const QString& discountRuleName,
         int createdBy,
         const QString& createdByLogin,
         double discountPercent,
         const QString& saleType,
         const QString& status);

    // Загрузка существующей сделки из базы данных
    Deal(int id,
         int customerId,
         const QString& customerName,
         int discountRuleId,
         const QString& discountRuleName,
         int createdBy,
         const QString& createdByLogin,
         double discountPercent,
         const QString& saleType,
         const QString& status,
         const QDateTime& createdAt);

    int id() const;

    int customerId() const;
    QString customerName() const;

    int discountRuleId() const;
    QString discountRuleName() const;

    int createdBy() const;
    QString createdByLogin() const;

    double subtotal() const;
    double discountPercent() const;
    double discountAmount() const;
    double totalAmount() const;

    QString saleType() const;
    QString status() const;

    QDateTime createdAt() const;

    const QVector<DealItem>& items() const;

    void setId(int id);

    void setCustomerId(int customerId);
    void setCustomerName(const QString& customerName);

    void setDiscountRuleId(int discountRuleId);
    void setDiscountRuleName(const QString& discountRuleName);

    void setCreatedBy(int createdBy);
    void setCreatedByLogin(const QString& createdByLogin);

    void setDiscountPercent(double discountPercent);
    void setSaleType(const QString& saleType);
    void setStatus(const QString& status);
    void setCreatedAt(const QDateTime& createdAt);

    void setItems(const QVector<DealItem>& items);

    bool addItem(const DealItem& item);
    bool updateItem(const DealItem& item);
    bool removeItemByProductId(int productId);

    void clearItems();

    bool containsProduct(int productId) const;
    bool isValid() const;

    static bool isSaleTypeValid(const QString& saleType);
    static bool isStatusValid(const QString& status);

private:
    int m_id;

    int m_customerId;
    QString m_customerName;

    int m_discountRuleId;
    QString m_discountRuleName;

    int m_createdBy;
    QString m_createdByLogin;

    double m_discountPercent;

    QString m_saleType;
    QString m_status;

    QDateTime m_createdAt;

    QVector<DealItem> m_items;

    static double roundMoney(double value);
};

#endif // DEAL_H
