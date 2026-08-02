#ifndef DISCOUNTRULEREPOSITORY_H
#define DISCOUNTRULEREPOSITORY_H

#include <QVector>
#include "../models/discountrule.h"

class DiscountRuleRepository
{
public:
    DiscountRuleRepository();

    QVector<DiscountRule> getAll() const;
    bool add(const DiscountRule& rule);
    bool update(const DiscountRule& rule);
    bool remove(int id);

private:
    bool ruleNameExists(const QString& name, int excludeId = -1) const;
};

#endif // DISCOUNTRULEREPOSITORY_H
