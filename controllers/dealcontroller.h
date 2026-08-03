#ifndef DEALCONTROLLER_H
#define DEALCONTROLLER_H

#include <QObject>
#include <QString>
#include <QVector>

#include "../models/customer.h"
#include "../models/deal.h"
#include "../models/discountrule.h"
#include "../models/product.h"
#include "../models/user.h"

#include "../repositories/customerrepository.h"
#include "../repositories/dealrepository.h"
#include "../repositories/discountrulerepository.h"
#include "../repositories/productrepository.h"

class DealController : public QObject
{
    Q_OBJECT

public:
    explicit DealController(
        const User& currentUser,
        QObject* parent = nullptr
        );

    // Справочники для заполнения QComboBox.
    QVector<Customer> getCustomers() const;
    QVector<Product> getProducts() const;
    QVector<DiscountRule> getDiscountRules() const;

    // Работа со сделками.
    QVector<Deal> getDeals();

    bool getDealById(
        int dealId,
        Deal& deal
        );

    bool createDeal(Deal& deal);
    bool updateDeal(Deal& deal);
    bool deleteDeal(int dealId);

    // Права текущего пользователя.
    bool canCreateDeals() const;
    bool canEditDeals() const;
    bool canDeleteDeals() const;

    const User& currentUser() const;

    QString lastError() const;

private:
    User m_currentUser;

    DealRepository m_dealRepository;
    CustomerRepository m_customerRepository;
    ProductRepository m_productRepository;
    DiscountRuleRepository m_discountRuleRepository;

    QString m_lastError;

    bool validateDeal(const Deal& deal);
    bool setError(const QString& message);
};

#endif // DEALCONTROLLER_H
