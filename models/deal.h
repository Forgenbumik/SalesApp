#ifndef DEAL_H
#define DEAL_H

#include <QString>

class Deal
{
public:
    Deal();
    Deal(int id,
         const QString& customerName,
         const QString& productName,
         int quantity,
         double totalAmount,
         const QString& status);

    int id() const;
    QString customerName() const;
    QString productName() const;
    int quantity() const;
    double totalAmount() const;
    QString status() const;

    void setId(int id);
    void setCustomerName(const QString& customerName);
    void setProductName(const QString& productName);
    void setQuantity(int quantity);
    void setTotalAmount(double totalAmount);
    void setStatus(const QString& status);

private:
    int m_id;
    QString m_customerName;
    QString m_productName;
    int m_quantity;
    double m_totalAmount;
    QString m_status;
};
#endif // DEAL_H
