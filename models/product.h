#ifndef PRODUCT_H
#define PRODUCT_H

#include <QString>

class Product
{
public:
    Product();
    Product(int id, const QString& name, double price, int quantity);

    int id() const;
    QString name() const;
    double price() const;
    int quantity() const;

    void setId(int id);
    void setName(const QString& name);
    void setPrice(double price);
    void setQuantity(int quantity);

private:
    int m_id;
    QString m_name;
    double m_price;
    int m_quantity;
};

#endif // PRODUCT_H
