#include "customerrepository.h"

CustomerRepository::CustomerRepository()
    : nextId(4)
{
    customers.append(Customer(1, "Иван Петров", "+79990001122", "ivan@example.com"));
    customers.append(Customer(2, "Анна Смирнова", "+79993334455", "anna@example.com"));
    customers.append(Customer(3, "Олег Кузнецов", "+79995556677", "oleg@example.com"));
}

QVector<Customer> CustomerRepository::getAll() const
{
    return customers;
}

void CustomerRepository::add(const Customer& customer)
{
    Customer newCustomer = customer;
    newCustomer.setId(nextId++);
    customers.append(newCustomer);
}

bool CustomerRepository::update(const Customer& customer)
{
    for (Customer& item : customers) {
        if (item.id() == customer.id()) {
            item = customer;
            return true;
        }
    }

    return false;
}

bool CustomerRepository::remove(int id)
{
    for (int i = 0; i < customers.size(); ++i) {
        if (customers[i].id() == id) {
            customers.removeAt(i);
            return true;
        }
    }

    return false;
}
