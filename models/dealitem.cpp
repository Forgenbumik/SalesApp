#include "dealitem.h"

#include <cmath>

DealItem::DealItem()
    : m_id(-1),
    m_dealId(-1),
    m_productId(-1),
    m_productName(""),
    m_quantity(0),
    m_unitPrice(0.0)
{
}

DealItem::DealItem(int productId,
                   const QString& productName,
                   int quantity,
                   double unitPrice)
    : m_id(-1),
    m_dealId(-1),
    m_productId(productId),
    m_productName(productName),
    m_quantity(quantity),
    m_unitPrice(unitPrice)
{
}

DealItem::DealItem(int id,
                   int dealId,
                   int productId,
                   const QString& productName,
                   int quantity,
                   double unitPrice)
    : m_id(id),
    m_dealId(dealId),
    m_productId(productId),
    m_productName(productName),
    m_quantity(quantity),
    m_unitPrice(unitPrice)
{
}

int DealItem::id() const
{
    return m_id;
}

int DealItem::dealId() const
{
    return m_dealId;
}

int DealItem::productId() const
{
    return m_productId;
}

QString DealItem::productName() const
{
    return m_productName;
}

int DealItem::quantity() const
{
    return m_quantity;
}

double DealItem::unitPrice() const
{
    return m_unitPrice;
}

double DealItem::lineTotal() const
{
    return roundMoney(
        m_unitPrice * static_cast<double>(m_quantity)
        );
}

void DealItem::setId(int id)
{
    m_id = id;
}

void DealItem::setDealId(int dealId)
{
    m_dealId = dealId;
}

void DealItem::setProductId(int productId)
{
    m_productId = productId;
}

void DealItem::setProductName(const QString& productName)
{
    m_productName = productName;
}

void DealItem::setQuantity(int quantity)
{
    m_quantity = quantity;
}

void DealItem::setUnitPrice(double unitPrice)
{
    m_unitPrice = unitPrice;
}

bool DealItem::isValid() const
{
    return m_productId > 0
           && !m_productName.trimmed().isEmpty()
           && m_quantity > 0
           && m_unitPrice >= 0.0;
}

double DealItem::roundMoney(double value)
{
    return std::round(value * 100.0) / 100.0;
}
