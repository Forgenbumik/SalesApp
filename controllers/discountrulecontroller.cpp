#include "discountrulecontroller.h"

DiscountRuleController::DiscountRuleController(QObject* parent)
    : QObject(parent)
{
}

QVector<DiscountRule> DiscountRuleController::getRules() const
{
    return repository.getAll();
}

bool DiscountRuleController::addRule(const QString& name, double minAmount, double discountPercent)
{
    if (!isValidRuleData(name, minAmount, discountPercent)) {
        return false;
    }

    DiscountRule rule(-1, name.trimmed(), minAmount, discountPercent);
    return repository.add(rule);
}

bool DiscountRuleController::updateRule(int id, const QString& name, double minAmount, double discountPercent)
{
    if (id <= 0 || !isValidRuleData(name, minAmount, discountPercent)) {
        return false;
    }

    DiscountRule rule(id, name.trimmed(), minAmount, discountPercent);
    return repository.update(rule);
}

bool DiscountRuleController::deleteRule(int id)
{
    if (id <= 0) {
        return false;
    }

    return repository.remove(id);
}

bool DiscountRuleController::isValidRuleData(const QString& name, double minAmount, double discountPercent) const
{
    if (name.trimmed().isEmpty()) {
        return false;
    }

    if (minAmount < 0) {
        return false;
    }

    if (discountPercent <= 0 || discountPercent > 100) {
        return false;
    }

    return true;
}
