#ifndef DISCOUNTRULECONTROLLER_H
#define DISCOUNTRULECONTROLLER_H

#include <QObject>
#include <QVector>

#include "../models/discountrule.h"
#include "../repositories/discountrulerepository.h"

class DiscountRuleController : public QObject
{
    Q_OBJECT

public:
    explicit DiscountRuleController(QObject* parent = nullptr);

    QVector<DiscountRule> getRules() const;

    bool addRule(const QString& name, double minAmount, double discountPercent);
    bool updateRule(int id, const QString& name, double minAmount, double discountPercent);
    bool deleteRule(int id);

private:
    DiscountRuleRepository repository;

    bool isValidRuleData(const QString& name, double minAmount, double discountPercent) const;
};

#endif // DISCOUNTRULECONTROLLER_H
