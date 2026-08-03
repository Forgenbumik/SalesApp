#ifndef DEALITEM_H
#define DEALITEM_H

#include <QString>

class DealItem
{
public:
    DealItem();

    // Создание новой строки ещё не сохранённой сделки.
    DealItem(int productId,
             const QString& productName,
             int quantity,
             double unitPrice);

    // Загрузка существующей строки из базы данных.
    DealItem(int id,
             int dealId,
             int productId,
             const QString& productName,
             int quantity,
             double unitPrice);

    int id() const;
    int dealId() const;
    int productId() const;

    QString productName() const;

    int quantity() const;
    double unitPrice() const;

    // Значение не хранится, а вычисляется.
    double lineTotal() const;

    void setId(int id);
    void setDealId(int dealId);
    void setProductId(int productId);
    void setProductName(const QString& productName);
    void setQuantity(int quantity);
    void setUnitPrice(double unitPrice);

    bool isValid() const;

private:
    int m_id;
    int m_dealId;
    int m_productId;

    QString m_productName;

    int m_quantity;
    double m_unitPrice;

    static double roundMoney(double value);
};

#endif // DEALITEM_H
