#include "discountrule.h"

DiscountRule::DiscountRule()
    : m_id(-1),
    m_name(""),
    m_minAmount(0.0),
    m_discountPercent(0.0)
{
}

DiscountRule::DiscountRule(int id, const QString& name, double minAmount, double discountPercent)
    : m_id(id),
    m_name(name),
    m_minAmount(minAmount),
    m_discountPercent(discountPercent)
{
}

int DiscountRule::id() const
{
    return m_id;
}

QString DiscountRule::name() const
{
    return m_name;
}

double DiscountRule::minAmount() const
{
    return m_minAmount;
}

double DiscountRule::discountPercent() const
{
    return m_discountPercent;
}

void DiscountRule::setId(int id)
{
    m_id = id;
}

void DiscountRule::setName(const QString& name)
{
    m_name = name;
}

void DiscountRule::setMinAmount(double minAmount)
{
    m_minAmount = minAmount;
}

void DiscountRule::setDiscountPercent(double discountPercent)
{
    m_discountPercent = discountPercent;
}
