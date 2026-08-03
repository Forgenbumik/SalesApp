#include "deal.h"

#include <QSet>

#include <cmath>

Deal::Deal()
    : m_id(-1),
    m_customerId(-1),
    m_customerName(""),
    m_discountRuleId(-1),
    m_discountRuleName("Без скидки"),
    m_createdBy(-1),
    m_createdByLogin(""),
    m_discountPercent(0.0),
    m_saleType("Розничная"),
    m_status("Новая"),
    m_createdAt()
{
}

Deal::Deal(int customerId,
           const QString& customerName,
           int discountRuleId,
           const QString& discountRuleName,
           int createdBy,
           const QString& createdByLogin,
           double discountPercent,
           const QString& saleType,
           const QString& status)
    : m_id(-1),
    m_customerId(customerId),
    m_customerName(customerName),
    m_discountRuleId(discountRuleId),
    m_discountRuleName(discountRuleName),
    m_createdBy(createdBy),
    m_createdByLogin(createdByLogin),
    m_discountPercent(discountPercent),
    m_saleType(saleType),
    m_status(status),
    m_createdAt()
{
}

Deal::Deal(int id,
           int customerId,
           const QString& customerName,
           int discountRuleId,
           const QString& discountRuleName,
           int createdBy,
           const QString& createdByLogin,
           double discountPercent,
           const QString& saleType,
           const QString& status,
           const QDateTime& createdAt)
    : m_id(id),
    m_customerId(customerId),
    m_customerName(customerName),
    m_discountRuleId(discountRuleId),
    m_discountRuleName(discountRuleName),
    m_createdBy(createdBy),
    m_createdByLogin(createdByLogin),
    m_discountPercent(discountPercent),
    m_saleType(saleType),
    m_status(status),
    m_createdAt(createdAt)
{
}

int Deal::id() const
{
    return m_id;
}

int Deal::customerId() const
{
    return m_customerId;
}

QString Deal::customerName() const
{
    return m_customerName;
}

int Deal::discountRuleId() const
{
    return m_discountRuleId;
}

QString Deal::discountRuleName() const
{
    return m_discountRuleName;
}

int Deal::createdBy() const
{
    return m_createdBy;
}

QString Deal::createdByLogin() const
{
    return m_createdByLogin;
}

double Deal::subtotal() const
{
    double result = 0.0;

    for (const DealItem& item : m_items) {
        result += item.lineTotal();
    }

    return roundMoney(result);
}

double Deal::discountPercent() const
{
    return m_discountPercent;
}

double Deal::discountAmount() const
{
    return roundMoney(
        subtotal() * m_discountPercent / 100.0
        );
}

double Deal::totalAmount() const
{
    return roundMoney(
        subtotal() - discountAmount()
        );
}

QString Deal::saleType() const
{
    return m_saleType;
}

QString Deal::status() const
{
    return m_status;
}

QDateTime Deal::createdAt() const
{
    return m_createdAt;
}

const QVector<DealItem>& Deal::items() const
{
    return m_items;
}

void Deal::setId(int id)
{
    m_id = id;

    for (DealItem& item : m_items) {
        item.setDealId(id);
    }
}

void Deal::setCustomerId(int customerId)
{
    m_customerId = customerId;
}

void Deal::setCustomerName(const QString& customerName)
{
    m_customerName = customerName;
}

void Deal::setDiscountRuleId(int discountRuleId)
{
    m_discountRuleId = discountRuleId;
}

void Deal::setDiscountRuleName(const QString& discountRuleName)
{
    m_discountRuleName = discountRuleName;
}

void Deal::setCreatedBy(int createdBy)
{
    m_createdBy = createdBy;
}

void Deal::setCreatedByLogin(const QString& createdByLogin)
{
    m_createdByLogin = createdByLogin;
}

void Deal::setDiscountPercent(double discountPercent)
{
    m_discountPercent = discountPercent;
}

void Deal::setSaleType(const QString& saleType)
{
    m_saleType = saleType;
}

void Deal::setStatus(const QString& status)
{
    m_status = status;
}

void Deal::setCreatedAt(const QDateTime& createdAt)
{
    m_createdAt = createdAt;
}

void Deal::setItems(const QVector<DealItem>& items)
{
    m_items = items;

    if (m_id > 0) {
        for (DealItem& item : m_items) {
            item.setDealId(m_id);
        }
    }
}

bool Deal::addItem(const DealItem& item)
{
    if (!item.isValid()) {
        return false;
    }

    if (containsProduct(item.productId())) {
        return false;
    }

    DealItem newItem = item;

    if (m_id > 0) {
        newItem.setDealId(m_id);
    }

    m_items.append(newItem);

    return true;
}

bool Deal::updateItem(const DealItem& item)
{
    if (!item.isValid()) {
        return false;
    }

    for (DealItem& currentItem : m_items) {
        if (currentItem.productId() == item.productId()) {
            DealItem updatedItem = item;

            if (m_id > 0) {
                updatedItem.setDealId(m_id);
            }

            currentItem = updatedItem;

            return true;
        }
    }

    return false;
}

bool Deal::removeItemByProductId(int productId)
{
    for (int index = 0; index < m_items.size(); ++index) {
        if (m_items[index].productId() == productId) {
            m_items.removeAt(index);
            return true;
        }
    }

    return false;
}

void Deal::clearItems()
{
    m_items.clear();
}

bool Deal::containsProduct(int productId) const
{
    for (const DealItem& item : m_items) {
        if (item.productId() == productId) {
            return true;
        }
    }

    return false;
}

bool Deal::isValid() const
{
    if (m_customerId <= 0) {
        return false;
    }

    if (m_customerName.trimmed().isEmpty()) {
        return false;
    }

    if (m_createdBy <= 0) {
        return false;
    }

    if (m_discountRuleId == 0 || m_discountRuleId < -1) {
        return false;
    }

    if (m_discountPercent < 0.0 || m_discountPercent > 100.0) {
        return false;
    }

    if (!isSaleTypeValid(m_saleType)) {
        return false;
    }

    if (!isStatusValid(m_status)) {
        return false;
    }

    if (m_items.isEmpty()) {
        return false;
    }

    QSet<int> productIds;

    for (const DealItem& item : m_items) {
        if (!item.isValid()) {
            return false;
        }

        if (productIds.contains(item.productId())) {
            return false;
        }

        productIds.insert(item.productId());
    }

    return true;
}

bool Deal::isSaleTypeValid(const QString& saleType)
{
    return saleType == "Розничная"
           || saleType == "Оптовая";
}

bool Deal::isStatusValid(const QString& status)
{
    return status == "Новая"
           || status == "Оформлена"
           || status == "Закрыта"
           || status == "Отменена";
}

double Deal::roundMoney(double value)
{
    return std::round(value * 100.0) / 100.0;
}
