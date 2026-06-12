#ifndef DEALCONTROLLER_H
#define DEALCONTROLLER_H

#include <QObject>
#include <QVector>

#include "../models/deal.h"
#include "../repositories/dealrepository.h"

class DealController : public QObject
{
    Q_OBJECT

public:
    explicit DealController(QObject* parent = nullptr);

    QVector<Deal> getDeals() const;

    bool addDeal(const QString& customerName,
                 const QString& productName,
                 int quantity,
                 double totalAmount,
                 const QString& status);

    bool updateDeal(int id,
                    const QString& customerName,
                    const QString& productName,
                    int quantity,
                    double totalAmount,
                    const QString& status);

    bool deleteDeal(int id);

private:
    DealRepository repository;

    bool isValidDealData(const QString& customerName,
                         const QString& productName,
                         int quantity,
                         double totalAmount,
                         const QString& status) const;
};

#endif // DEALCONTROLLER_H
