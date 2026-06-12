#include "product.h"

Product::Product()
    : m_id(-1), m_name(""), m_price(0.0), m_quantity(0)
{
}

Product::Product(int id, const QString& name, double price, int quantity)
    : m_id(id), m_name(name), m_price(price), m_quantity(quantity)
{
}

int Product::id() const
{
    return m_id;
}

QString Product::name() const
{
    return m_name;
}

double Product::price() const
{
    return m_price;
}

int Product::quantity() const
{
    return m_quantity;
}

void Product::setId(int id)
{
    m_id = id;
}

void Product::setName(const QString& name)
{
    m_name = name;
}

void Product::setPrice(double price)
{
    m_price = price;
}

void Product::setQuantity(int quantity)
{
    m_quantity = quantity;
}
