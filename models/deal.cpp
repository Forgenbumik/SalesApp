#include "deal.h"

Deal::Deal()
    : m_id(-1),
    m_customerName(""),
    m_productName(""),
    m_quantity(0),
    m_totalAmount(0.0),
    m_status("")
{
}

Deal::Deal(int id,
           const QString& customerName,
           const QString& productName,
           int quantity,
           double totalAmount,
           const QString& status)
    : m_id(id),
    m_customerName(customerName),
    m_productName(productName),
    m_quantity(quantity),
    m_totalAmount(totalAmount),
    m_status(status)
{
}

int Deal::id() const
{
    return m_id;
}

QString Deal::customerName() const
{
    return m_customerName;
}

QString Deal::productName() const
{
    return m_productName;
}

int Deal::quantity() const
{
    return m_quantity;
}

double Deal::totalAmount() const
{
    return m_totalAmount;
}

QString Deal::status() const
{
    return m_status;
}

void Deal::setId(int id)
{
    m_id = id;
}

void Deal::setCustomerName(const QString& customerName)
{
    m_customerName = customerName;
}

void Deal::setProductName(const QString& productName)
{
    m_productName = productName;
}

void Deal::setQuantity(int quantity)
{
    m_quantity = quantity;
}

void Deal::setTotalAmount(double totalAmount)
{
    m_totalAmount = totalAmount;
}

void Deal::setStatus(const QString& status)
{
    m_status = status;
}
