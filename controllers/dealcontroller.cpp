#include "dealcontroller.h"

#include <QSet>

DealController::DealController(
    const User& currentUser,
    QObject* parent
    )
    : QObject(parent),
    m_currentUser(currentUser)
{
}

// Загрузка справочников
QVector<Customer> DealController::getCustomers() const
{
    return m_customerRepository.getAll();
}

QVector<Product> DealController::getProducts() const
{
    return m_productRepository.getAll();
}

QVector<DiscountRule> DealController::getDiscountRules() const
{
    return m_discountRuleRepository.getAll();
}

// Загрузка сделок
QVector<Deal> DealController::getDeals()
{
    m_lastError.clear();

    QVector<Deal> deals =
        m_dealRepository.getAll(m_currentUser);

    const QString repositoryError =
        m_dealRepository.lastError();

    if (!repositoryError.isEmpty()) {
        m_lastError = repositoryError;
    }

    return deals;
}

bool DealController::getDealById(
    int dealId,
    Deal& deal
    )
{
    m_lastError.clear();

    if (dealId <= 0) {
        return setError(
            "Некорректный идентификатор сделки"
            );
    }

    if (!m_dealRepository.findById(
            dealId,
            m_currentUser,
            deal)) {
        return setError(
            m_dealRepository.lastError()
            );
    }

    return true;
}

// Создание сделки
bool DealController::createDeal(Deal& deal)
{
    m_lastError.clear();

    if (!canCreateDeals()) {
        return setError(
            "У пользователя нет права создавать сделки"
            );
    }

    deal.setCreatedBy(m_currentUser.id());
    deal.setCreatedByLogin(m_currentUser.login());

    if (!m_currentUser.isAdmin()) {
        deal.setStatus("Новая");
    }

    if (!validateDeal(deal)) {
        return false;
    }

    if (!m_dealRepository.add(
            deal,
            m_currentUser)) {
        return setError(
            m_dealRepository.lastError()
            );
    }

    return true;
}

// Изменение сделки
bool DealController::updateDeal(Deal& deal)
{
    m_lastError.clear();

    if (!canEditDeals()) {
        return setError(
            "Изменять сделки может только администратор"
            );
    }

    if (deal.id() <= 0) {
        return setError(
            "Некорректный идентификатор сделки"
            );
    }

    if (!validateDeal(deal)) {
        return false;
    }

    if (!m_dealRepository.update(
            deal,
            m_currentUser)) {
        return setError(
            m_dealRepository.lastError()
            );
    }

    return true;
}

// Удаление сделки
bool DealController::deleteDeal(int dealId)
{
    m_lastError.clear();

    if (!canDeleteDeals()) {
        return setError(
            "Удалять сделки может только администратор"
            );
    }

    if (dealId <= 0) {
        return setError(
            "Некорректный идентификатор сделки"
            );
    }

    if (!m_dealRepository.remove(
            dealId,
            m_currentUser)) {
        return setError(
            m_dealRepository.lastError()
            );
    }

    return true;
}

// Проверка прав
bool DealController::canCreateDeals() const
{
    return m_currentUser.id() > 0;
}

bool DealController::canEditDeals() const
{
    return m_currentUser.id() > 0
           && m_currentUser.isAdmin();
}

bool DealController::canDeleteDeals() const
{
    return m_currentUser.id() > 0
           && m_currentUser.isAdmin();
}

const User& DealController::currentUser() const
{
    return m_currentUser;
}

// Проверка сделки
bool DealController::validateDeal(const Deal& deal)
{
    if (m_currentUser.id() <= 0) {
        return setError(
            "Текущий пользователь не определён"
            );
    }

    if (deal.customerId() <= 0) {
        return setError(
            "Не выбран покупатель"
            );
    }

    if (deal.customerName().trimmed().isEmpty()) {
        return setError(
            "Не указано имя покупателя"
            );
    }

    if (deal.discountRuleId() == 0
        || deal.discountRuleId() < -1) {
        return setError(
            "Некорректное правило скидки"
            );
    }

    if (deal.discountPercent() < 0.0
        || deal.discountPercent() > 100.0) {
        return setError(
            "Процент скидки должен находиться "
            "в диапазоне от 0 до 100"
            );
    }

    if (!Deal::isSaleTypeValid(deal.saleType())) {
        return setError(
            "Некорректный тип продажи"
            );
    }

    if (!Deal::isStatusValid(deal.status())) {
        return setError(
            "Некорректный статус сделки"
            );
    }

    if (deal.items().isEmpty()) {
        return setError(
            "Добавьте в сделку хотя бы один товар"
            );
    }

    QSet<int> productIds;

    for (const DealItem& item : deal.items()) {
        if (!item.isValid()) {
            return setError(
                "В составе сделки обнаружена "
                "некорректная строка товара"
                );
        }

        if (productIds.contains(item.productId())) {
            return setError(
                "Один товар нельзя добавить "
                "в сделку несколько раз"
                );
        }

        productIds.insert(item.productId());
    }

    return true;
}

// Ошибки
QString DealController::lastError() const
{
    return m_lastError;
}

bool DealController::setError(
    const QString& message
    )
{
    if (message.trimmed().isEmpty()) {
        m_lastError =
            "Произошла неизвестная ошибка";
    } else {
        m_lastError = message;
    }

    return false;
}
