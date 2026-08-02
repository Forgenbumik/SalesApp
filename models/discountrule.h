#ifndef DISCOUNTRULE_H
#define DISCOUNTRULE_H

#include <QString>

class DiscountRule
{
public:
    DiscountRule();
    DiscountRule(int id, const QString& name, double minAmount, double discountPercent);

    int id() const;
    QString name() const;
    double minAmount() const;
    double discountPercent() const;

    void setId(int id);
    void setName(const QString& name);
    void setMinAmount(double minAmount);
    void setDiscountPercent(double discountPercent);

private:
    int m_id;
    QString m_name;
    double m_minAmount;
    double m_discountPercent;
};

#endif // DISCOUNTRULE_H
