#include "dealswindow.h"

#include <QAbstractItemView>
#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QSignalBlocker>
#include <QVBoxLayout>

#include <cmath>

namespace
{
double roundMoney(double value)
{
    return std::round(value * 100.0) / 100.0;
}

QString moneyText(double value)
{
    return QString::number(
               roundMoney(value),
               'f',
               2
               )
           + " ₽";
}
}

DealsWindow::DealsWindow(
    const User& currentUser,
    QWidget* parent
    )
    : QWidget(parent),
    m_currentUser(currentUser),
    m_controller(
        new DealController(
            currentUser,
            this
            )
        ),
    m_selectedDealId(-1),
    m_selectedItemRow(-1)
{
    setupUi();
    loadReferenceData();
    loadDeals();
    clearDealForm();
}

void DealsWindow::setupUi()
{
    setWindowTitle("Сделки");
    resize(1200, 850);

    // Таблица документов сделок
    m_dealsTable = new QTableWidget(this);
    m_dealsTable->setColumnCount(9);

    m_dealsTable->setHorizontalHeaderLabels(
        {
            "ID",
            "Покупатель",
            "Тип продажи",
            "Скидка",
            "Сумма",
            "Итого",
            "Статус",
            "Создал",
            "Дата"
        }
        );

    m_dealsTable->setSelectionBehavior(
        QAbstractItemView::SelectRows
        );

    m_dealsTable->setSelectionMode(
        QAbstractItemView::SingleSelection
        );

    m_dealsTable->setEditTriggers(
        QAbstractItemView::NoEditTriggers
        );

    m_dealsTable->verticalHeader()->setVisible(false);

    m_dealsTable->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch
        );

    QGroupBox* dealsGroup =
        new QGroupBox("Список сделок", this);

    QVBoxLayout* dealsGroupLayout =
        new QVBoxLayout(dealsGroup);

    dealsGroupLayout->addWidget(m_dealsTable);

    // Основные поля сделки
    m_customerComboBox = new QComboBox(this);
    m_discountComboBox = new QComboBox(this);
    m_saleTypeComboBox = new QComboBox(this);
    m_statusComboBox = new QComboBox(this);

    m_saleTypeComboBox->addItems(
        {
            "Розничная",
            "Оптовая"
        }
        );

    m_statusComboBox->addItems(
        {
            "Новая",
            "Оформлена",
            "Закрыта",
            "Отменена"
        }
        );

    QFormLayout* dealFormLayout =
        new QFormLayout();

    dealFormLayout->addRow(
        "Покупатель:",
        m_customerComboBox
        );

    dealFormLayout->addRow(
        "Тип продажи:",
        m_saleTypeComboBox
        );

    dealFormLayout->addRow(
        "Правило скидки:",
        m_discountComboBox
        );

    dealFormLayout->addRow(
        "Статус:",
        m_statusComboBox
        );

    m_discountHintLabel = new QLabel(this);
    m_discountHintLabel->setWordWrap(true);

    dealFormLayout->addRow(
        "Условие скидки:",
        m_discountHintLabel
        );

    QGroupBox* dealDataGroup =
        new QGroupBox("Данные сделки", this);

    dealDataGroup->setLayout(dealFormLayout);

    // Добавление товара
    m_productComboBox = new QComboBox(this);

    m_quantitySpinBox = new QSpinBox(this);
    m_quantitySpinBox->setMinimum(1);
    m_quantitySpinBox->setMaximum(1000000);
    m_quantitySpinBox->setValue(1);

    m_productPriceLabel = new QLabel("0.00 ₽", this);
    m_productStockLabel = new QLabel("0", this);
    m_lineTotalLabel = new QLabel("0.00 ₽", this);

    QFormLayout* itemFormLayout =
        new QFormLayout();

    itemFormLayout->addRow(
        "Товар:",
        m_productComboBox
        );

    itemFormLayout->addRow(
        "Цена:",
        m_productPriceLabel
        );

    itemFormLayout->addRow(
        "Остаток:",
        m_productStockLabel
        );

    itemFormLayout->addRow(
        "Количество:",
        m_quantitySpinBox
        );

    itemFormLayout->addRow(
        "Сумма строки:",
        m_lineTotalLabel
        );

    m_addItemButton =
        new QPushButton("Добавить товар", this);

    m_updateItemButton =
        new QPushButton("Изменить строку", this);

    m_removeItemButton =
        new QPushButton("Удалить строку", this);

    QHBoxLayout* itemButtonsLayout =
        new QHBoxLayout();

    itemButtonsLayout->addWidget(m_addItemButton);
    itemButtonsLayout->addWidget(m_updateItemButton);
    itemButtonsLayout->addWidget(m_removeItemButton);

    QGroupBox* itemInputGroup =
        new QGroupBox("Добавление товара", this);

    QVBoxLayout* itemInputLayout =
        new QVBoxLayout(itemInputGroup);

    itemInputLayout->addLayout(itemFormLayout);
    itemInputLayout->addLayout(itemButtonsLayout);

    // Таблица состава сделки
    m_itemsTable = new QTableWidget(this);
    m_itemsTable->setColumnCount(4);

    m_itemsTable->setHorizontalHeaderLabels(
        {
            "Товар",
            "Цена",
            "Количество",
            "Сумма"
        }
        );

    m_itemsTable->setSelectionBehavior(
        QAbstractItemView::SelectRows
        );

    m_itemsTable->setSelectionMode(
        QAbstractItemView::SingleSelection
        );

    m_itemsTable->setEditTriggers(
        QAbstractItemView::NoEditTriggers
        );

    m_itemsTable->verticalHeader()->setVisible(false);

    m_itemsTable->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch
        );

    QGroupBox* itemsGroup =
        new QGroupBox("Состав сделки", this);

    QVBoxLayout* itemsLayout =
        new QVBoxLayout(itemsGroup);

    itemsLayout->addWidget(m_itemsTable);

    // Итоги
    m_subtotalLabel = new QLabel("0.00 ₽", this);
    m_discountPercentLabel = new QLabel("0.00 %", this);
    m_discountAmountLabel = new QLabel("0.00 ₽", this);
    m_totalAmountLabel = new QLabel("0.00 ₽", this);

    QFont totalFont = m_totalAmountLabel->font();
    totalFont.setBold(true);
    totalFont.setPointSize(
        totalFont.pointSize() + 2
        );

    m_totalAmountLabel->setFont(totalFont);

    QGridLayout* totalsLayout =
        new QGridLayout();

    totalsLayout->addWidget(
        new QLabel("Сумма без скидки:", this),
        0,
        0
        );

    totalsLayout->addWidget(
        m_subtotalLabel,
        0,
        1
        );

    totalsLayout->addWidget(
        new QLabel("Процент скидки:", this),
        1,
        0
        );

    totalsLayout->addWidget(
        m_discountPercentLabel,
        1,
        1
        );

    totalsLayout->addWidget(
        new QLabel("Размер скидки:", this),
        2,
        0
        );

    totalsLayout->addWidget(
        m_discountAmountLabel,
        2,
        1
        );

    totalsLayout->addWidget(
        new QLabel("Итоговая стоимость:", this),
        3,
        0
        );

    totalsLayout->addWidget(
        m_totalAmountLabel,
        3,
        1
        );

    QGroupBox* totalsGroup =
        new QGroupBox("Итоги", this);

    totalsGroup->setLayout(totalsLayout);

    // Кнопки работы со сделкой
    m_createDealButton =
        new QPushButton("Создать сделку", this);

    m_updateDealButton =
        new QPushButton("Сохранить изменения", this);

    m_deleteDealButton =
        new QPushButton("Удалить сделку", this);

    m_newDealButton =
        new QPushButton("Новая сделка", this);

    m_refreshButton =
        new QPushButton("Обновить данные", this);

    QHBoxLayout* dealButtonsLayout =
        new QHBoxLayout();

    dealButtonsLayout->addWidget(m_newDealButton);
    dealButtonsLayout->addWidget(m_createDealButton);
    dealButtonsLayout->addWidget(m_updateDealButton);
    dealButtonsLayout->addWidget(m_deleteDealButton);
    dealButtonsLayout->addStretch();
    dealButtonsLayout->addWidget(m_refreshButton);

    m_messageLabel = new QLabel(this);
    m_messageLabel->setWordWrap(true);
    m_messageLabel->setStyleSheet(
        "color: #b00020;"
        );

    // Общая компоновка
    QHBoxLayout* editorLayout =
        new QHBoxLayout();

    QVBoxLayout* leftEditorLayout =
        new QVBoxLayout();

    leftEditorLayout->addWidget(dealDataGroup);
    leftEditorLayout->addWidget(itemInputGroup);
    leftEditorLayout->addWidget(totalsGroup);
    leftEditorLayout->addStretch();

    editorLayout->addLayout(
        leftEditorLayout,
        1
        );

    editorLayout->addWidget(
        itemsGroup,
        2
        );

    QVBoxLayout* mainLayout =
        new QVBoxLayout(this);

    mainLayout->addWidget(dealsGroup, 2);
    mainLayout->addLayout(editorLayout, 3);
    mainLayout->addLayout(dealButtonsLayout);
    mainLayout->addWidget(m_messageLabel);

    setLayout(mainLayout);

    // Сигналы
    connect(
        m_dealsTable,
        &QTableWidget::cellClicked,
        this,
        &DealsWindow::onDealRowClicked
        );

    connect(
        m_itemsTable,
        &QTableWidget::cellClicked,
        this,
        &DealsWindow::onItemRowClicked
        );

    connect(
        m_productComboBox,
        qOverload<int>(
            &QComboBox::currentIndexChanged
            ),
        this,
        &DealsWindow::onProductChanged
        );

    connect(
        m_quantitySpinBox,
        qOverload<int>(
            &QSpinBox::valueChanged
            ),
        this,
        &DealsWindow::onQuantityChanged
        );

    connect(
        m_discountComboBox,
        qOverload<int>(
            &QComboBox::currentIndexChanged
            ),
        this,
        &DealsWindow::onDiscountChanged
        );

    connect(
        m_addItemButton,
        &QPushButton::clicked,
        this,
        &DealsWindow::onAddItemClicked
        );

    connect(
        m_updateItemButton,
        &QPushButton::clicked,
        this,
        &DealsWindow::onUpdateItemClicked
        );

    connect(
        m_removeItemButton,
        &QPushButton::clicked,
        this,
        &DealsWindow::onRemoveItemClicked
        );

    connect(
        m_createDealButton,
        &QPushButton::clicked,
        this,
        &DealsWindow::onCreateDealClicked
        );

    connect(
        m_updateDealButton,
        &QPushButton::clicked,
        this,
        &DealsWindow::onUpdateDealClicked
        );

    connect(
        m_deleteDealButton,
        &QPushButton::clicked,
        this,
        &DealsWindow::onDeleteDealClicked
        );

    connect(
        m_newDealButton,
        &QPushButton::clicked,
        this,
        &DealsWindow::onNewDealClicked
        );

    connect(
        m_refreshButton,
        &QPushButton::clicked,
        this,
        &DealsWindow::onRefreshClicked
        );
}

// Загрузка покупателей, товаров и скидок
void DealsWindow::loadReferenceData()
{
    m_customers =
        m_controller->getCustomers();

    m_products =
        m_controller->getProducts();

    m_discountRules =
        m_controller->getDiscountRules();

    const QSignalBlocker customerBlocker(
        m_customerComboBox
        );

    const QSignalBlocker productBlocker(
        m_productComboBox
        );

    const QSignalBlocker discountBlocker(
        m_discountComboBox
        );

    m_customerComboBox->clear();
    m_productComboBox->clear();
    m_discountComboBox->clear();

    for (const Customer& customer : m_customers) {
        m_customerComboBox->addItem(
            customer.fullName(),
            customer.id()
            );
    }

    for (const Product& product : m_products) {
        const QString visibleText =
            product.name()
            + " — "
            + moneyText(product.price());

        m_productComboBox->addItem(
            visibleText,
            product.id()
            );
    }

    m_discountComboBox->addItem(
        "Без скидки",
        -1
        );

    for (const DiscountRule& rule : m_discountRules) {
        const QString visibleText =
            rule.name()
            + " — "
            + QString::number(
                rule.discountPercent(),
                'f',
                2
                )
            + "%";

        m_discountComboBox->addItem(
            visibleText,
            rule.id()
            );
    }

    if (m_customers.isEmpty()) {
        m_customerComboBox->addItem(
            "Нет доступных покупателей",
            -1
            );
    }

    if (m_products.isEmpty()) {
        m_productComboBox->addItem(
            "Нет доступных товаров",
            -1
            );
    }

    updateProductInformation();
    recalculateTotals();
}

// Загрузка сделок
void DealsWindow::loadDeals()
{
    QVector<Deal> deals =
        m_controller->getDeals();

    m_dealsTable->setRowCount(
        deals.size()
        );

    for (int row = 0;
         row < deals.size();
         ++row) {
        const Deal& deal = deals[row];

        QTableWidgetItem* idItem =
            new QTableWidgetItem(
                QString::number(deal.id())
                );

        idItem->setData(
            Qt::UserRole,
            deal.id()
            );

        m_dealsTable->setItem(
            row,
            0,
            idItem
            );

        m_dealsTable->setItem(
            row,
            1,
            new QTableWidgetItem(
                deal.customerName()
                )
            );

        m_dealsTable->setItem(
            row,
            2,
            new QTableWidgetItem(
                deal.saleType()
                )
            );

        m_dealsTable->setItem(
            row,
            3,
            new QTableWidgetItem(
                deal.discountRuleName()
                )
            );

        m_dealsTable->setItem(
            row,
            4,
            new QTableWidgetItem(
                moneyText(deal.subtotal())
                )
            );

        m_dealsTable->setItem(
            row,
            5,
            new QTableWidgetItem(
                moneyText(deal.totalAmount())
                )
            );

        m_dealsTable->setItem(
            row,
            6,
            new QTableWidgetItem(
                deal.status()
                )
            );

        m_dealsTable->setItem(
            row,
            7,
            new QTableWidgetItem(
                deal.createdByLogin()
                )
            );

        m_dealsTable->setItem(
            row,
            8,
            new QTableWidgetItem(
                deal.createdAt()
                    .toLocalTime()
                    .toString("dd.MM.yyyy HH:mm")
                )
            );
    }

    const QString error =
        m_controller->lastError();

    if (!error.isEmpty()) {
        showError(error);
    }
}

// Выбор сделки
void DealsWindow::onDealRowClicked(
    int row,
    int column
    )
{
    Q_UNUSED(column);

    QTableWidgetItem* idItem =
        m_dealsTable->item(row, 0);

    if (idItem == nullptr) {
        return;
    }

    const int dealId =
        idItem->data(Qt::UserRole).toInt();

    Deal deal;

    if (!m_controller->getDealById(
            dealId,
            deal)) {
        showError(
            m_controller->lastError()
            );

        return;
    }

    clearMessage();

    m_selectedDealId = deal.id();
    m_selectedItemRow = -1;
    m_items = deal.items();

    const int customerIndex =
        m_customerComboBox->findData(
            deal.customerId()
            );

    if (customerIndex >= 0) {
        m_customerComboBox->setCurrentIndex(
            customerIndex
            );
    }

    const int discountIndex =
        m_discountComboBox->findData(
            deal.discountRuleId()
            );

    if (discountIndex >= 0) {
        m_discountComboBox->setCurrentIndex(
            discountIndex
            );
    } else {
        m_discountComboBox->setCurrentIndex(0);
    }

    const int saleTypeIndex =
        m_saleTypeComboBox->findText(
            deal.saleType()
            );

    if (saleTypeIndex >= 0) {
        m_saleTypeComboBox->setCurrentIndex(
            saleTypeIndex
            );
    }

    const int statusIndex =
        m_statusComboBox->findText(
            deal.status()
            );

    if (statusIndex >= 0) {
        m_statusComboBox->setCurrentIndex(
            statusIndex
            );
    }

    m_quantitySpinBox->setValue(1);

    populateItemsTable();
    updateProductInformation();
    recalculateTotals();
    updateUiMode();
}

// Работа с составом
void DealsWindow::populateItemsTable()
{
    m_itemsTable->setRowCount(
        m_items.size()
        );

    for (int row = 0;
         row < m_items.size();
         ++row) {
        const DealItem& item = m_items[row];

        QTableWidgetItem* productItem =
            new QTableWidgetItem(
                item.productName()
                );

        productItem->setData(
            Qt::UserRole,
            item.productId()
            );

        m_itemsTable->setItem(
            row,
            0,
            productItem
            );

        m_itemsTable->setItem(
            row,
            1,
            new QTableWidgetItem(
                moneyText(item.unitPrice())
                )
            );

        m_itemsTable->setItem(
            row,
            2,
            new QTableWidgetItem(
                QString::number(item.quantity())
                )
            );

        m_itemsTable->setItem(
            row,
            3,
            new QTableWidgetItem(
                moneyText(item.lineTotal())
                )
            );
    }
}

void DealsWindow::onItemRowClicked(
    int row,
    int column
    )
{
    Q_UNUSED(column);

    if (row < 0 || row >= m_items.size()) {
        return;
    }

    m_selectedItemRow = row;

    const DealItem& item =
        m_items[row];

    const int productIndex =
        m_productComboBox->findData(
            item.productId()
            );

    if (productIndex >= 0) {
        m_productComboBox->setCurrentIndex(
            productIndex
            );
    }

    m_quantitySpinBox->setValue(
        item.quantity()
        );

    m_productPriceLabel->setText(
        moneyText(item.unitPrice())
        );

    m_lineTotalLabel->setText(
        moneyText(item.lineTotal())
        );
}

void DealsWindow::onAddItemClicked()
{
    clearMessage();

    const int productId =
        m_productComboBox->currentData().toInt();

    const Product* product =
        findProduct(productId);

    if (product == nullptr) {
        showError("Выберите товар");
        return;
    }

    const int quantity =
        m_quantitySpinBox->value();

    if (quantity <= 0) {
        showError(
            "Количество должно быть больше нуля"
            );

        return;
    }

    for (const DealItem& currentItem : m_items) {
        if (currentItem.productId() == productId) {
            showError(
                "Этот товар уже добавлен в сделку. "
                "Выберите строку и измените количество."
                );

            return;
        }
    }

    if (m_selectedDealId <= 0
        && quantity > product->quantity()) {
        showError(
            "Недостаточно товара на складе. Доступно: "
            + QString::number(product->quantity())
            );

        return;
    }

    DealItem item(
        product->id(),
        product->name(),
        quantity,
        product->price()
        );

    m_items.append(item);

    m_selectedItemRow = -1;
    m_quantitySpinBox->setValue(1);
    m_itemsTable->clearSelection();

    populateItemsTable();
    updateProductInformation();
    recalculateTotals();
}

void DealsWindow::onUpdateItemClicked()
{
    clearMessage();

    if (m_selectedItemRow < 0
        || m_selectedItemRow >= m_items.size()) {
        showError(
            "Выберите строку товара для изменения"
            );

        return;
    }

    const int productId =
        m_productComboBox->currentData().toInt();

    const Product* product =
        findProduct(productId);

    if (product == nullptr) {
        showError("Выберите товар");
        return;
    }

    const int quantity =
        m_quantitySpinBox->value();

    if (quantity <= 0) {
        showError(
            "Количество должно быть больше нуля"
            );

        return;
    }

    for (int index = 0;
         index < m_items.size();
         ++index) {
        if (index != m_selectedItemRow
            && m_items[index].productId()
                   == productId) {
            showError(
                "Этот товар уже присутствует "
                "в другой строке сделки"
                );

            return;
        }
    }

    DealItem updatedItem(
        product->id(),
        product->name(),
        quantity,
        product->price()
        );

    m_items[m_selectedItemRow] =
        updatedItem;

    m_selectedItemRow = -1;
    m_quantitySpinBox->setValue(1);
    m_itemsTable->clearSelection();

    populateItemsTable();
    updateProductInformation();
    recalculateTotals();
}

void DealsWindow::onRemoveItemClicked()
{
    clearMessage();

    if (m_selectedItemRow < 0
        || m_selectedItemRow >= m_items.size()) {
        showError(
            "Выберите строку товара для удаления"
            );

        return;
    }

    m_items.removeAt(
        m_selectedItemRow
        );

    m_selectedItemRow = -1;
    m_quantitySpinBox->setValue(1);
    m_itemsTable->clearSelection();

    populateItemsTable();
    updateProductInformation();
    recalculateTotals();
}

// Автоматический расчёт
void DealsWindow::onProductChanged(int index)
{
    Q_UNUSED(index);
    updateProductInformation();
}

void DealsWindow::onQuantityChanged(int quantity)
{
    Q_UNUSED(quantity);
    updateProductInformation();
}

void DealsWindow::onDiscountChanged(int index)
{
    Q_UNUSED(index);
    recalculateTotals();
}

void DealsWindow::updateProductInformation()
{
    const int productId =
        m_productComboBox->currentData().toInt();

    const Product* product =
        findProduct(productId);

    if (product == nullptr) {
        m_productPriceLabel->setText("0.00 ₽");
        m_productStockLabel->setText("0");
        m_lineTotalLabel->setText("0.00 ₽");

        return;
    }

    m_productPriceLabel->setText(
        moneyText(product->price())
        );

    m_productStockLabel->setText(
        QString::number(product->quantity())
        );

    const double lineTotal =
        product->price()
        * static_cast<double>(
            m_quantitySpinBox->value()
            );

    m_lineTotalLabel->setText(
        moneyText(lineTotal)
        );
}

void DealsWindow::recalculateTotals()
{
    double subtotal = 0.0;

    for (const DealItem& item : m_items) {
        subtotal += item.lineTotal();
    }

    subtotal = roundMoney(subtotal);

    const int discountRuleId =
        m_discountComboBox->currentData().toInt();

    const DiscountRule* rule =
        findDiscountRule(discountRuleId);

    double appliedPercent = 0.0;

    if (rule == nullptr) {
        m_discountHintLabel->setText(
            "Скидка не применяется"
            );

        m_discountHintLabel->setStyleSheet("");
    } else if (subtotal >= rule->minAmount()) {
        appliedPercent =
            rule->discountPercent();

        m_discountHintLabel->setText(
            "Скидка применяется"
            );

        m_discountHintLabel->setStyleSheet(
            "color: green;"
            );
    } else {
        const double missingAmount =
            roundMoney(
                rule->minAmount() - subtotal
                );

        m_discountHintLabel->setText(
            "Для применения скидки не хватает "
            + moneyText(missingAmount)
            );

        m_discountHintLabel->setStyleSheet(
            "color: #b00020;"
            );
    }

    const double discountAmount =
        roundMoney(
            subtotal * appliedPercent / 100.0
            );

    const double totalAmount =
        roundMoney(
            subtotal - discountAmount
            );

    m_subtotalLabel->setText(
        moneyText(subtotal)
        );

    m_discountPercentLabel->setText(
        QString::number(
            appliedPercent,
            'f',
            2
            )
        + " %"
        );

    m_discountAmountLabel->setText(
        moneyText(discountAmount)
        );

    m_totalAmountLabel->setText(
        moneyText(totalAmount)
        );
}

// Создание объекта Deal из формы
Deal DealsWindow::buildDealFromForm() const
{
    const int customerId =
        m_customerComboBox->currentData().toInt();

    QString customerName;

    for (const Customer& customer : m_customers) {
        if (customer.id() == customerId) {
            customerName =
                customer.fullName();

            break;
        }
    }

    const int discountRuleId =
        m_discountComboBox->currentData().toInt();

    QString discountRuleName =
        "Без скидки";

    double discountPercent = 0.0;

    const DiscountRule* discountRule =
        findDiscountRule(discountRuleId);

    if (discountRule != nullptr) {
        discountRuleName =
            discountRule->name();

        discountPercent =
            discountRule->discountPercent();
    }

    Deal deal(
        customerId,
        customerName,
        discountRuleId,
        discountRuleName,
        m_currentUser.id(),
        m_currentUser.login(),
        discountPercent,
        m_saleTypeComboBox->currentText(),
        m_statusComboBox->currentText()
        );

    if (m_selectedDealId > 0) {
        deal.setId(
            m_selectedDealId
            );
    }

    deal.setItems(m_items);

    return deal;
}

// Сохранение сделки
void DealsWindow::onCreateDealClicked()
{
    clearMessage();

    if (m_selectedDealId > 0) {
        showError(
            "Сейчас открыта существующая сделка. "
            "Нажмите «Новая сделка»."
            );

        return;
    }

    Deal deal =
        buildDealFromForm();

    if (!m_controller->createDeal(deal)) {
        showError(
            m_controller->lastError()
            );

        return;
    }

    QMessageBox::information(
        this,
        "Сделка создана",
        "Сделка успешно создана."
        );

    loadReferenceData();
    loadDeals();
    clearDealForm();
}

void DealsWindow::onUpdateDealClicked()
{
    clearMessage();

    if (m_selectedDealId <= 0) {
        showError(
            "Выберите сделку для изменения"
            );

        return;
    }

    Deal deal =
        buildDealFromForm();

    if (!m_controller->updateDeal(deal)) {
        showError(
            m_controller->lastError()
            );

        return;
    }

    QMessageBox::information(
        this,
        "Сделка изменена",
        "Изменения успешно сохранены."
        );

    loadReferenceData();
    loadDeals();
    clearDealForm();
}

void DealsWindow::onDeleteDealClicked()
{
    clearMessage();

    if (m_selectedDealId <= 0) {
        showError(
            "Выберите сделку для удаления"
            );

        return;
    }

    const QMessageBox::StandardButton answer =
        QMessageBox::question(
            this,
            "Удаление сделки",
            "Удалить выбранную сделку?\n"
            "Списанный товар будет возвращён на склад.",
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No
            );

    if (answer != QMessageBox::Yes) {
        return;
    }

    if (!m_controller->deleteDeal(
            m_selectedDealId)) {
        showError(
            m_controller->lastError()
            );

        return;
    }

    QMessageBox::information(
        this,
        "Сделка удалена",
        "Сделка успешно удалена."
        );

    loadReferenceData();
    loadDeals();
    clearDealForm();
}

// Очистка и обновление
void DealsWindow::onNewDealClicked()
{
    clearDealForm();
}

void DealsWindow::onRefreshClicked()
{
    loadReferenceData();
    loadDeals();
    clearDealForm();
}

void DealsWindow::clearDealForm()
{
    m_selectedDealId = -1;
    m_selectedItemRow = -1;

    m_items.clear();

    if (m_customerComboBox->count() > 0) {
        m_customerComboBox->setCurrentIndex(0);
    }

    if (m_discountComboBox->count() > 0) {
        m_discountComboBox->setCurrentIndex(0);
    }

    const int retailIndex =
        m_saleTypeComboBox->findText(
            "Розничная"
            );

    if (retailIndex >= 0) {
        m_saleTypeComboBox->setCurrentIndex(
            retailIndex
            );
    }

    const int newStatusIndex =
        m_statusComboBox->findText("Новая");

    if (newStatusIndex >= 0) {
        m_statusComboBox->setCurrentIndex(
            newStatusIndex
            );
    }

    if (m_productComboBox->count() > 0) {
        m_productComboBox->setCurrentIndex(0);
    }

    m_quantitySpinBox->setValue(1);

    m_dealsTable->clearSelection();
    m_itemsTable->clearSelection();

    populateItemsTable();
    updateProductInformation();
    recalculateTotals();
    updateUiMode();
    clearMessage();
}

// Права и состояние формы
void DealsWindow::updateUiMode()
{
    const bool existingDeal =
        m_selectedDealId > 0;

    const bool canEditExistingDeal =
        existingDeal
        && m_controller->canEditDeals();

    const bool formEnabled =
        !existingDeal
        || canEditExistingDeal;

    m_customerComboBox->setEnabled(
        formEnabled
        );

    m_discountComboBox->setEnabled(
        formEnabled
        );

    m_saleTypeComboBox->setEnabled(
        formEnabled
        );

    m_productComboBox->setEnabled(
        formEnabled
        );

    m_quantitySpinBox->setEnabled(
        formEnabled
        );

    m_addItemButton->setEnabled(
        formEnabled
        );

    m_updateItemButton->setEnabled(
        formEnabled
        );

    m_removeItemButton->setEnabled(
        formEnabled
        );

    m_statusComboBox->setEnabled(
        formEnabled
        && m_currentUser.isAdmin()
        );

    m_createDealButton->setEnabled(
        !existingDeal
        && m_controller->canCreateDeals()
        );

    m_updateDealButton->setEnabled(
        canEditExistingDeal
        );

    m_deleteDealButton->setEnabled(
        existingDeal
        && m_controller->canDeleteDeals()
        );
}

// Поиск справочных объектов
const Product* DealsWindow::findProduct(
    int productId
    ) const
{
    for (const Product& product : m_products) {
        if (product.id() == productId) {
            return &product;
        }
    }

    return nullptr;
}

const DiscountRule*
DealsWindow::findDiscountRule(
    int ruleId
    ) const
{
    if (ruleId <= 0) {
        return nullptr;
    }

    for (const DiscountRule& rule :
         m_discountRules) {
        if (rule.id() == ruleId) {
            return &rule;
        }
    }

    return nullptr;
}

// Сообщения
void DealsWindow::showError(
    const QString& message
    )
{
    if (message.trimmed().isEmpty()) {
        m_messageLabel->setText(
            "Произошла неизвестная ошибка"
            );
    } else {
        m_messageLabel->setText(message);
    }
}

void DealsWindow::clearMessage()
{
    m_messageLabel->clear();
}
