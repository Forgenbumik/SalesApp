#include "dealrepository.h"

#include <QDebug>
#include <QSet>
#include <QSqlError>
#include <QVariant>

#include <algorithm>
#include <cmath>

DealRepository::DealRepository()
{
}

// Получение списка сделок
QVector<Deal> DealRepository::getAll(
    const User& currentUser
    ) const
{
    m_lastError.clear();

    QVector<Deal> deals;

    QSqlDatabase db =
        QSqlDatabase::database("main_connection");

    if (!db.isValid() || !db.isOpen()) {
        setError(
            "Соединение с базой данных отсутствует"
            );

        return deals;
    }

    QString sql =
        "SELECT "
        "d.id, "
        "d.customer_id, "
        "c.full_name AS customer_name, "
        "d.discount_rule_id, "
        "COALESCE(dr.name, 'Без скидки') "
        "AS discount_rule_name, "
        "d.created_by, "
        "u.login AS created_by_login, "
        "d.discount_percent, "
        "d.sale_type, "
        "d.status, "
        "d.created_at "
        "FROM deals d "
        "JOIN customers c "
        "ON c.id = d.customer_id "
        "JOIN users u "
        "ON u.id = d.created_by "
        "LEFT JOIN discount_rules dr "
        "ON dr.id = d.discount_rule_id ";

    if (!currentUser.isAdmin()) {
        sql +=
            "WHERE d.created_by = "
            ":current_user_id ";
    }

    sql += "ORDER BY d.id DESC";

    QSqlQuery query(db);
    query.prepare(sql);

    if (!currentUser.isAdmin()) {
        query.bindValue(
            ":current_user_id",
            currentUser.id()
            );
    }

    if (!query.exec()) {
        setQueryError(
            "Ошибка загрузки сделок",
            query
            );

        return {};
    }

    while (query.next()) {
        const QVariant discountRuleIdValue =
            query.value("discount_rule_id");

        const int discountRuleId =
            discountRuleIdValue.isNull()
                ? -1
                : discountRuleIdValue.toInt();

        deals.append(
            Deal(
                query.value("id").toInt(),
                query.value("customer_id").toInt(),
                query.value("customer_name").toString(),
                discountRuleId,
                query.value("discount_rule_name").toString(),
                query.value("created_by").toInt(),
                query.value("created_by_login").toString(),
                query.value("discount_percent").toDouble(),
                query.value("sale_type").toString(),
                query.value("status").toString(),
                query.value("created_at").toDateTime()
                )
            );
    }

    query.finish();

    // После загрузки состава модель Deal сама вычисляет суммы.
    for (Deal& deal : deals) {
        QVector<DealItem> items;

        if (!loadItems(
                deal.id(),
                db,
                items)) {
            return {};
        }

        deal.setItems(items);
    }

    return deals;
}

// Получение одной сделки
bool DealRepository::findById(
    int dealId,
    const User& currentUser,
    Deal& deal
    ) const
{
    m_lastError.clear();

    if (dealId <= 0) {
        return setError(
            "Некорректный идентификатор сделки"
            );
    }

    QSqlDatabase db =
        QSqlDatabase::database("main_connection");

    if (!db.isValid() || !db.isOpen()) {
        return setError(
            "Соединение с базой данных отсутствует"
            );
    }

    QString sql =
        "SELECT "
        "d.id, "
        "d.customer_id, "
        "c.full_name AS customer_name, "
        "d.discount_rule_id, "
        "COALESCE(dr.name, 'Без скидки') "
        "AS discount_rule_name, "
        "d.created_by, "
        "u.login AS created_by_login, "
        "d.discount_percent, "
        "d.sale_type, "
        "d.status, "
        "d.created_at "
        "FROM deals d "
        "JOIN customers c "
        "ON c.id = d.customer_id "
        "JOIN users u "
        "ON u.id = d.created_by "
        "LEFT JOIN discount_rules dr "
        "ON dr.id = d.discount_rule_id "
        "WHERE d.id = :deal_id ";

    if (!currentUser.isAdmin()) {
        sql +=
            "AND d.created_by = "
            ":current_user_id ";
    }

    QSqlQuery query(db);
    query.prepare(sql);

    query.bindValue(
        ":deal_id",
        dealId
        );

    if (!currentUser.isAdmin()) {
        query.bindValue(
            ":current_user_id",
            currentUser.id()
            );
    }

    if (!query.exec()) {
        return setQueryError(
            "Ошибка загрузки сделки",
            query
            );
    }

    if (!query.next()) {
        return setError(
            "Сделка не найдена или "
            "недоступна пользователю"
            );
    }

    const QVariant discountRuleIdValue =
        query.value("discount_rule_id");

    const int discountRuleId =
        discountRuleIdValue.isNull()
            ? -1
            : discountRuleIdValue.toInt();

    Deal loadedDeal(
        query.value("id").toInt(),
        query.value("customer_id").toInt(),
        query.value("customer_name").toString(),
        discountRuleId,
        query.value("discount_rule_name").toString(),
        query.value("created_by").toInt(),
        query.value("created_by_login").toString(),
        query.value("discount_percent").toDouble(),
        query.value("sale_type").toString(),
        query.value("status").toString(),
        query.value("created_at").toDateTime()
        );

    query.finish();

    QVector<DealItem> items;

    if (!loadItems(
            dealId,
            db,
            items)) {
        return false;
    }

    loadedDeal.setItems(items);
    deal = loadedDeal;

    return true;
}

// Создание сделки
bool DealRepository::add(
    Deal& deal,
    const User& currentUser
    )
{
    m_lastError.clear();

    if (currentUser.id() <= 0) {
        return setError(
            "Текущий пользователь не определён"
            );
    }

    if (deal.customerId() <= 0) {
        return setError(
            "Не выбран покупатель"
            );
    }

    if (deal.items().isEmpty()) {
        return setError(
            "Сделка должна содержать "
            "хотя бы один товар"
            );
    }

    if (!Deal::isSaleTypeValid(deal.saleType())) {
        return setError(
            "Некорректный тип продажи"
            );
    }

    if (!Deal::isStatusValid(
            deal.status())) {
        return setError(
            "Некорректный статус сделки"
            );
    }

    if (!currentUser.isAdmin()
        && deal.status() != "Новая") {
        return setError(
            "Обычный пользователь может "
            "создать сделку только со "
            "статусом «Новая»"
            );
    }

    QSqlDatabase db =
        QSqlDatabase::database("main_connection");

    if (!db.isValid() || !db.isOpen()) {
        return setError(
            "Соединение с базой данных отсутствует"
            );
    }

    if (!db.transaction()) {
        return setError(
            "Не удалось начать транзакцию: "
            + db.lastError().text()
            );
    }

    QString customerName;

    if (!findCustomerName(
            deal.customerId(),
            db,
            customerName)) {
        db.rollback();
        return false;
    }

    QVector<DealItem> preparedItems;

    // Промежуточная сумма нужна только для проверки скидки.
    double subtotal = 0.0;

    const bool reserveStock =
        usesStock(deal.status());

    if (!prepareItems(
            deal.items(),
            reserveStock,
            db,
            preparedItems,
            subtotal)) {
        db.rollback();
        return false;
    }

    QString discountRuleName;
    double discountPercent = 0.0;

    if (!resolveDiscount(
            deal.discountRuleId(),
            subtotal,
            db,
            discountRuleName,
            discountPercent)) {
        db.rollback();
        return false;
    }

    QSqlQuery insertDealQuery(db);

    insertDealQuery.prepare(
        "INSERT INTO deals ("
        "customer_id, "
        "discount_rule_id, "
        "created_by, "
        "discount_percent, "
        "sale_type, "
        "status"
        ") "
        "VALUES ("
        ":customer_id, "
        ":discount_rule_id, "
        ":created_by, "
        ":discount_percent, "
        ":status"
        ") "
        "RETURNING id, created_at"
        );

    insertDealQuery.bindValue(
        ":customer_id",
        deal.customerId()
        );

    if (deal.discountRuleId() > 0) {
        insertDealQuery.bindValue(
            ":discount_rule_id",
            deal.discountRuleId()
            );
    } else {
        insertDealQuery.bindValue(
            ":discount_rule_id",
            QVariant()
            );
    }

    insertDealQuery.bindValue(
        ":created_by",
        currentUser.id()
        );

    insertDealQuery.bindValue(
        ":discount_percent",
        discountPercent
        );

    insertDealQuery.bindValue(
        ":sale_type",
        deal.saleType()
        );

    insertDealQuery.bindValue(
        ":status",
        deal.status()
        );

    if (!insertDealQuery.exec()) {
        setQueryError(
            "Ошибка создания документа сделки",
            insertDealQuery
            );

        db.rollback();
        return false;
    }

    if (!insertDealQuery.next()) {
        setError(
            "База данных не вернула "
            "идентификатор сделки"
            );

        db.rollback();
        return false;
    }

    const int dealId =
        insertDealQuery.value("id").toInt();

    const QDateTime createdAt =
        insertDealQuery.value(
                           "created_at"
                           ).toDateTime();

    if (!insertItems(
            dealId,
            db,
            preparedItems)) {
        db.rollback();
        return false;
    }

    if (!db.commit()) {
        const QString error =
            db.lastError().text();

        db.rollback();

        return setError(
            "Не удалось сохранить транзакцию: "
            + error
            );
    }

    deal.setId(dealId);
    deal.setCustomerName(customerName);

    deal.setCreatedBy(
        currentUser.id()
        );

    deal.setCreatedByLogin(
        currentUser.login()
        );

    deal.setDiscountRuleId(
        deal.discountRuleId() > 0
            ? deal.discountRuleId()
            : -1
        );

    deal.setDiscountRuleName(
        discountRuleName
        );

    deal.setDiscountPercent(
        discountPercent
        );

    deal.setItems(preparedItems);
    deal.setCreatedAt(createdAt);

    return true;
}

// Изменение сделки
bool DealRepository::update(
    Deal& deal,
    const User& currentUser
    )
{
    m_lastError.clear();

    if (!currentUser.isAdmin()) {
        return setError(
            "Изменять сделки может "
            "только администратор"
            );
    }

    if (deal.id() <= 0) {
        return setError(
            "Некорректный идентификатор сделки"
            );
    }

    if (deal.customerId() <= 0) {
        return setError(
            "Не выбран покупатель"
            );
    }

    if (deal.items().isEmpty()) {
        return setError(
            "Сделка должна содержать "
            "хотя бы один товар"
            );
    }

    if (!Deal::isSaleTypeValid(deal.saleType())) {
        return setError(
            "Некорректный тип продажи"
            );
    }

    if (!Deal::isStatusValid(
            deal.status())) {
        return setError(
            "Некорректный статус сделки"
            );
    }

    QSqlDatabase db =
        QSqlDatabase::database("main_connection");

    if (!db.isValid() || !db.isOpen()) {
        return setError(
            "Соединение с базой данных отсутствует"
            );
    }

    if (!db.transaction()) {
        return setError(
            "Не удалось начать транзакцию: "
            + db.lastError().text()
            );
    }

    QSqlQuery oldDealQuery(db);

    oldDealQuery.prepare(
        "SELECT "
        "d.status, "
        "d.created_by, "
        "u.login AS created_by_login, "
        "d.created_at "
        "FROM deals d "
        "JOIN users u "
        "ON u.id = d.created_by "
        "WHERE d.id = :deal_id "
        "FOR UPDATE"
        );

    oldDealQuery.bindValue(
        ":deal_id",
        deal.id()
        );

    if (!oldDealQuery.exec()) {
        setQueryError(
            "Ошибка загрузки изменяемой сделки",
            oldDealQuery
            );

        db.rollback();
        return false;
    }

    if (!oldDealQuery.next()) {
        setError(
            "Изменяемая сделка не найдена"
            );

        db.rollback();
        return false;
    }

    const QString oldStatus =
        oldDealQuery.value(
                        "status"
                        ).toString();

    const int createdBy =
        oldDealQuery.value(
                        "created_by"
                        ).toInt();

    const QString createdByLogin =
        oldDealQuery.value(
                        "created_by_login"
                        ).toString();

    const QDateTime createdAt =
        oldDealQuery.value(
                        "created_at"
                        ).toDateTime();

    oldDealQuery.finish();

    QVector<DealItem> oldItems;

    if (!loadItems(
            deal.id(),
            db,
            oldItems)) {
        db.rollback();
        return false;
    }

    // Возвращаем на склад товары из старого состава сделки.
    if (usesStock(oldStatus)) {
        if (!restoreStock(
                oldItems,
                db)) {
            db.rollback();
            return false;
        }
    }

    QSqlQuery deleteItemsQuery(db);

    deleteItemsQuery.prepare(
        "DELETE FROM deal_items "
        "WHERE deal_id = :deal_id"
        );

    deleteItemsQuery.bindValue(
        ":deal_id",
        deal.id()
        );

    if (!deleteItemsQuery.exec()) {
        setQueryError(
            "Ошибка удаления старого "
            "состава сделки",
            deleteItemsQuery
            );

        db.rollback();
        return false;
    }

    QString customerName;

    if (!findCustomerName(
            deal.customerId(),
            db,
            customerName)) {
        db.rollback();
        return false;
    }

    QVector<DealItem> preparedItems;
    double subtotal = 0.0;

    const bool reserveNewStock =
        usesStock(deal.status());

    if (!prepareItems(
            deal.items(),
            reserveNewStock,
            db,
            preparedItems,
            subtotal)) {
        db.rollback();
        return false;
    }

    QString discountRuleName;
    double discountPercent = 0.0;

    if (!resolveDiscount(
            deal.discountRuleId(),
            subtotal,
            db,
            discountRuleName,
            discountPercent)) {
        db.rollback();
        return false;
    }

    QSqlQuery updateDealQuery(db);

    updateDealQuery.prepare(
        "UPDATE deals "
        "SET customer_id = :customer_id, "
        "discount_rule_id = :discount_rule_id, "
        "discount_percent = :discount_percent, "
        "sale_type = :sale_type, "
        "status = :status "
        "WHERE id = :deal_id"
        );

    updateDealQuery.bindValue(
        ":customer_id",
        deal.customerId()
        );

    if (deal.discountRuleId() > 0) {
        updateDealQuery.bindValue(
            ":discount_rule_id",
            deal.discountRuleId()
            );
    } else {
        updateDealQuery.bindValue(
            ":discount_rule_id",
            QVariant()
            );
    }

    updateDealQuery.bindValue(
        ":discount_percent",
        discountPercent
        );

    updateDealQuery.bindValue(
        ":sale_type",
        deal.saleType()
        );

    updateDealQuery.bindValue(
        ":status",
        deal.status()
        );

    updateDealQuery.bindValue(
        ":deal_id",
        deal.id()
        );

    if (!updateDealQuery.exec()) {
        setQueryError(
            "Ошибка изменения документа сделки",
            updateDealQuery
            );

        db.rollback();
        return false;
    }

    if (updateDealQuery.numRowsAffected() <= 0) {
        setError(
            "Сделка не была изменена"
            );

        db.rollback();
        return false;
    }

    if (!insertItems(
            deal.id(),
            db,
            preparedItems)) {
        db.rollback();
        return false;
    }

    if (!db.commit()) {
        const QString error =
            db.lastError().text();

        db.rollback();

        return setError(
            "Не удалось сохранить изменения: "
            + error
            );
    }

    deal.setCustomerName(customerName);

    deal.setCreatedBy(createdBy);
    deal.setCreatedByLogin(
        createdByLogin
        );

    deal.setDiscountRuleId(
        deal.discountRuleId() > 0
            ? deal.discountRuleId()
            : -1
        );

    deal.setDiscountRuleName(
        discountRuleName
        );

    deal.setDiscountPercent(
        discountPercent
        );

    deal.setItems(preparedItems);
    deal.setCreatedAt(createdAt);

    return true;
}

// Удаление сделки
bool DealRepository::remove(
    int dealId,
    const User& currentUser
    )
{
    m_lastError.clear();

    if (!currentUser.isAdmin()) {
        return setError(
            "Удалять сделки может "
            "только администратор"
            );
    }

    if (dealId <= 0) {
        return setError(
            "Некорректный идентификатор сделки"
            );
    }

    QSqlDatabase db =
        QSqlDatabase::database("main_connection");

    if (!db.isValid() || !db.isOpen()) {
        return setError(
            "Соединение с базой данных отсутствует"
            );
    }

    if (!db.transaction()) {
        return setError(
            "Не удалось начать транзакцию: "
            + db.lastError().text()
            );
    }

    QSqlQuery statusQuery(db);

    statusQuery.prepare(
        "SELECT status "
        "FROM deals "
        "WHERE id = :deal_id "
        "FOR UPDATE"
        );

    statusQuery.bindValue(
        ":deal_id",
        dealId
        );

    if (!statusQuery.exec()) {
        setQueryError(
            "Ошибка загрузки удаляемой сделки",
            statusQuery
            );

        db.rollback();
        return false;
    }

    if (!statusQuery.next()) {
        setError(
            "Удаляемая сделка не найдена"
            );

        db.rollback();
        return false;
    }

    const QString status =
        statusQuery.value(
                       "status"
                       ).toString();

    statusQuery.finish();

    QVector<DealItem> items;

    if (!loadItems(
            dealId,
            db,
            items)) {
        db.rollback();
        return false;
    }

    if (usesStock(status)) {
        if (!restoreStock(
                items,
                db)) {
            db.rollback();
            return false;
        }
    }

    QSqlQuery deleteQuery(db);

    deleteQuery.prepare(
        "DELETE FROM deals "
        "WHERE id = :deal_id"
        );

    deleteQuery.bindValue(
        ":deal_id",
        dealId
        );

    if (!deleteQuery.exec()) {
        setQueryError(
            "Ошибка удаления сделки",
            deleteQuery
            );

        db.rollback();
        return false;
    }

    if (deleteQuery.numRowsAffected() <= 0) {
        setError(
            "Сделка не была удалена"
            );

        db.rollback();
        return false;
    }

    // Строки deal_items удаляются через ON DELETE CASCADE.
    if (!db.commit()) {
        const QString error =
            db.lastError().text();

        db.rollback();

        return setError(
            "Не удалось завершить удаление: "
            + error
            );
    }

    return true;
}

// Загрузка состава сделки
bool DealRepository::loadItems(
    int dealId,
    const QSqlDatabase& db,
    QVector<DealItem>& items
    ) const
{
    items.clear();

    QSqlQuery query(db);

    query.prepare(
        "SELECT "
        "di.id, "
        "di.deal_id, "
        "di.product_id, "
        "p.name AS product_name, "
        "di.quantity, "
        "di.unit_price "
        "FROM deal_items di "
        "JOIN products p "
        "ON p.id = di.product_id "
        "WHERE di.deal_id = :deal_id "
        "ORDER BY di.id"
        );

    query.bindValue(
        ":deal_id",
        dealId
        );

    if (!query.exec()) {
        return setQueryError(
            "Ошибка загрузки состава сделки",
            query
            );
    }

    while (query.next()) {
        items.append(
            DealItem(
                query.value("id").toInt(),
                query.value("deal_id").toInt(),
                query.value("product_id").toInt(),
                query.value(
                         "product_name"
                         ).toString(),
                query.value("quantity").toInt(),
                query.value(
                         "unit_price"
                         ).toDouble()
                )
            );
    }

    return true;
}

// Получение покупателя
bool DealRepository::findCustomerName(
    int customerId,
    const QSqlDatabase& db,
    QString& customerName
    )
{
    QSqlQuery query(db);

    query.prepare(
        "SELECT full_name "
        "FROM customers "
        "WHERE id = :customer_id"
        );

    query.bindValue(
        ":customer_id",
        customerId
        );

    if (!query.exec()) {
        return setQueryError(
            "Ошибка поиска покупателя",
            query
            );
    }

    if (!query.next()) {
        return setError(
            "Выбранный покупатель не найден"
            );
    }

    customerName =
        query.value(
                 "full_name"
                 ).toString();

    return true;
}

// Подготовка товаров и изменение остатков
bool DealRepository::prepareItems(
    const QVector<DealItem>& requestedItems,
    bool reserveStock,
    const QSqlDatabase& db,
    QVector<DealItem>& preparedItems,
    double& subtotal
    )
{
    preparedItems.clear();
    subtotal = 0.0;

    if (requestedItems.isEmpty()) {
        return setError(
            "Состав сделки не может быть пустым"
            );
    }

    QVector<DealItem> sortedItems =
        requestedItems;

    std::sort(
        sortedItems.begin(),
        sortedItems.end(),
        [](const DealItem& first,
           const DealItem& second) {
            return first.productId()
            < second.productId();
        }
        );

    QSet<int> productIds;

    for (const DealItem& requestedItem :
         sortedItems) {
        if (requestedItem.productId() <= 0) {
            return setError(
                "В составе указан "
                "некорректный товар"
                );
        }

        if (requestedItem.quantity() <= 0) {
            return setError(
                "Количество товара должно "
                "быть больше нуля"
                );
        }

        if (productIds.contains(
                requestedItem.productId())) {
            return setError(
                "Один товар нельзя добавить "
                "в сделку несколько раз"
                );
        }

        productIds.insert(
            requestedItem.productId()
            );

        QSqlQuery productQuery(db);

        productQuery.prepare(
            "SELECT name, price, quantity "
            "FROM products "
            "WHERE id = :product_id "
            "FOR UPDATE"
            );

        productQuery.bindValue(
            ":product_id",
            requestedItem.productId()
            );

        if (!productQuery.exec()) {
            return setQueryError(
                "Ошибка получения товара",
                productQuery
                );
        }

        if (!productQuery.next()) {
            return setError(
                "Один из выбранных "
                "товаров не найден"
                );
        }

        const QString productName =
            productQuery.value(
                            "name"
                            ).toString();

        const double unitPrice =
            roundMoney(
                productQuery.value(
                                "price"
                                ).toDouble()
                );

        const int availableQuantity =
            productQuery.value(
                            "quantity"
                            ).toInt();

        if (reserveStock
            && availableQuantity
                   < requestedItem.quantity()) {
            return setError(
                "Недостаточно товара «"
                + productName
                + "». Доступно: "
                + QString::number(
                    availableQuantity
                    )
                );
        }

        DealItem preparedItem(
            requestedItem.productId(),
            productName,
            requestedItem.quantity(),
            unitPrice
            );

        if (reserveStock) {
            QSqlQuery stockQuery(db);

            stockQuery.prepare(
                "UPDATE products "
                "SET quantity = "
                "quantity - :quantity "
                "WHERE id = :product_id "
                "AND quantity >= :quantity"
                );

            stockQuery.bindValue(
                ":quantity",
                requestedItem.quantity()
                );

            stockQuery.bindValue(
                ":product_id",
                requestedItem.productId()
                );

            if (!stockQuery.exec()) {
                return setQueryError(
                    "Ошибка списания товара",
                    stockQuery
                    );
            }

            if (stockQuery.numRowsAffected()
                <= 0) {
                return setError(
                    "Не удалось списать товар «"
                    + productName
                    + "»"
                    );
            }
        }

        preparedItems.append(
            preparedItem
            );

        subtotal +=
            preparedItem.lineTotal();
    }

    subtotal = roundMoney(subtotal);

    return true;
}

// Проверка правила скидки
bool DealRepository::resolveDiscount(
    int discountRuleId,
    double subtotal,
    const QSqlDatabase& db,
    QString& discountRuleName,
    double& discountPercent
    )
{
    if (discountRuleId <= 0) {
        discountRuleName =
            "Без скидки";

        discountPercent = 0.0;

        return true;
    }

    QSqlQuery query(db);

    query.prepare(
        "SELECT "
        "name, "
        "min_amount, "
        "discount_percent "
        "FROM discount_rules "
        "WHERE id = :discount_rule_id"
        );

    query.bindValue(
        ":discount_rule_id",
        discountRuleId
        );

    if (!query.exec()) {
        return setQueryError(
            "Ошибка получения правила скидки",
            query
            );
    }

    if (!query.next()) {
        return setError(
            "Выбранное правило скидки "
            "не найдено"
            );
    }

    const double minAmount =
        query.value(
                 "min_amount"
                 ).toDouble();

    if (subtotal + 0.001 < minAmount) {
        return setError(
            "Скидка «"
            + query.value("name").toString()
            + "» применяется от суммы "
            + QString::number(
                minAmount,
                'f',
                2
                )
            );
    }

    discountRuleName =
        query.value(
                 "name"
                 ).toString();

    discountPercent =
        query.value(
                 "discount_percent"
                 ).toDouble();

    return true;
}

// Сохранение состава сделки
bool DealRepository::insertItems(
    int dealId,
    const QSqlDatabase& db,
    QVector<DealItem>& items
    )
{
    for (DealItem& item : items) {
        QSqlQuery query(db);

        query.prepare(
            "INSERT INTO deal_items ("
            "deal_id, "
            "product_id, "
            "quantity, "
            "unit_price"
            ") "
            "VALUES ("
            ":deal_id, "
            ":product_id, "
            ":quantity, "
            ":unit_price"
            ") "
            "RETURNING id"
            );

        query.bindValue(
            ":deal_id",
            dealId
            );

        query.bindValue(
            ":product_id",
            item.productId()
            );

        query.bindValue(
            ":quantity",
            item.quantity()
            );

        query.bindValue(
            ":unit_price",
            item.unitPrice()
            );

        if (!query.exec()) {
            return setQueryError(
                "Ошибка сохранения "
                "состава сделки",
                query
                );
        }

        if (!query.next()) {
            return setError(
                "Не получен идентификатор "
                "строки сделки"
                );
        }

        item.setId(
            query.value("id").toInt()
            );

        item.setDealId(dealId);
    }

    return true;
}

// Возврат товара на склад
bool DealRepository::restoreStock(
    const QVector<DealItem>& items,
    const QSqlDatabase& db
    )
{
    QVector<DealItem> sortedItems =
        items;

    std::sort(
        sortedItems.begin(),
        sortedItems.end(),
        [](const DealItem& first,
           const DealItem& second) {
            return first.productId()
            < second.productId();
        }
        );

    for (const DealItem& item :
         sortedItems) {
        QSqlQuery query(db);

        query.prepare(
            "UPDATE products "
            "SET quantity = "
            "quantity + :quantity "
            "WHERE id = :product_id"
            );

        query.bindValue(
            ":quantity",
            item.quantity()
            );

        query.bindValue(
            ":product_id",
            item.productId()
            );

        if (!query.exec()) {
            return setQueryError(
                "Ошибка возврата товара на склад",
                query
                );
        }

        if (query.numRowsAffected() <= 0) {
            return setError(
                "Не удалось вернуть товар "
                "с идентификатором "
                + QString::number(
                    item.productId()
                    )
                );
        }
    }

    return true;
}

// Ошибки и вспомогательные методы
QString DealRepository::lastError() const
{
    return m_lastError;
}

bool DealRepository::setError(
    const QString& message
    ) const
{
    m_lastError = message;

    qDebug()
        << "DealRepository:"
        << message;

    return false;
}

bool DealRepository::setQueryError(
    const QString& context,
    const QSqlQuery& query
    ) const
{
    return setError(
        context
        + ": "
        + query.lastError().text()
        );
}

bool DealRepository::usesStock(
    const QString& status
    )
{
    return status != "Отменена";
}

double DealRepository::roundMoney(
    double value
    )
{
    return std::round(
               value * 100.0
               )
           / 100.0;
}
