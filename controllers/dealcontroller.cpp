#include "dealcontroller.h"

DealController::DealController(QObject* parent)
    : QObject(parent)
{
}

QVector<Deal> DealController::getDeals() const
{
    return repository.getAll();
}

bool DealController::addDeal(const QString& customerName,
                             const QString& productName,
                             int quantity,
                             double totalAmount,
                             const QString& status)
{
    if (!isValidDealData(customerName, productName, quantity, totalAmount, status)) {
        return false;
    }

    Deal deal(-1,
              customerName.trimmed(),
              productName.trimmed(),
              quantity,
              totalAmount,
              status.trimmed());

    repository.add(deal);

    return true;
}

bool DealController::updateDeal(int id,
                                const QString& customerName,
                                const QString& productName,
                                int quantity,
                                double totalAmount,
                                const QString& status)
{
    if (id <= 0 || !isValidDealData(customerName, productName, quantity, totalAmount, status)) {
        return false;
    }

    Deal deal(id,
              customerName.trimmed(),
              productName.trimmed(),
              quantity,
              totalAmount,
              status.trimmed());

    return repository.update(deal);
}

bool DealController::deleteDeal(int id)
{
    if (id <= 0) {
        return false;
    }

    return repository.remove(id);
}

bool DealController::isValidDealData(const QString& customerName,
                                     const QString& productName,
                                     int quantity,
                                     double totalAmount,
                                     const QString& status) const
{
    if (customerName.trimmed().isEmpty()) {
        return false;
    }

    if (productName.trimmed().isEmpty()) {
        return false;
    }

    if (quantity <= 0) {
        return false;
    }

    if (totalAmount <= 0) {
        return false;
    }

    if (status.trimmed().isEmpty()) {
        return false;
    }

    return true;
}
