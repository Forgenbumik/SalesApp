#include "customercontroller.h"
#include <QRegularExpression>

CustomerController::CustomerController(QObject* parent)
    : QObject(parent)
{
}

QVector<Customer> CustomerController::getCustomers() const
{
    return repository.getAll();
}

bool CustomerController::addCustomer(const QString& fullName, const QString& phone, const QString& email)
{
    if (!isValidCustomerData(fullName, phone, email)) {
        return false;
    }

    Customer customer(
        -1,
        fullName.trimmed(),
        phone.trimmed(),
        email.trimmed()
        );

    return repository.add(customer);
}

bool CustomerController::updateCustomer(int id, const QString& fullName, const QString& phone, const QString& email)
{
    if (id <= 0 || !isValidCustomerData(fullName, phone, email)) {
        return false;
    }

    Customer customer(
        id,
        fullName.trimmed(),
        phone.trimmed(),
        email.trimmed()
        );

    return repository.update(customer);
}

bool CustomerController::deleteCustomer(int id)
{
    if (id <= 0) {
        return false;
    }

    return repository.remove(id);
}

bool CustomerController::isValidCustomerData(const QString& fullName, const QString& phone, const QString& email) const
{
    if (fullName.trimmed().isEmpty()) {
        return false;
    }

    QRegularExpression phoneRegex("^\\+?[0-9]{10,15}$");
    if (!phoneRegex.match(phone.trimmed()).hasMatch()) {
        return false;
    }

    QRegularExpression emailRegex("^[\\w\\.\\-]+@[\\w\\.-]+\\.[A-Za-z]{2,}$");
    if (!emailRegex.match(email.trimmed()).hasMatch()) {
        return false;
    }

    return true;
}
