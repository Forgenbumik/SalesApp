#include "dealrepository.h"

DealRepository::DealRepository()
    : nextId(4)
{
    deals.append(Deal(1, "Иван Петров", "Ноутбук", 1, 75000.0, "Новая"));
    deals.append(Deal(2, "Анна Смирнова", "Смартфон", 2, 90000.0, "Оформлена"));
    deals.append(Deal(3, "Олег Кузнецов", "Наушники", 3, 24000.0, "Закрыта"));
}

QVector<Deal> DealRepository::getAll() const
{
    return deals;
}

void DealRepository::add(const Deal& deal)
{
    Deal newDeal = deal;
    newDeal.setId(nextId++);
    deals.append(newDeal);
}

bool DealRepository::update(const Deal& deal)
{
    for (Deal& item : deals) {
        if (item.id() == deal.id()) {
            item = deal;
            return true;
        }
    }

    return false;
}

bool DealRepository::remove(int id)
{
    for (int i = 0; i < deals.size(); ++i) {
        if (deals[i].id() == id) {
            deals.removeAt(i);
            return true;
        }
    }

    return false;
}
