#include "dealswindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QHeaderView>

DealsWindow::DealsWindow(const User& user, QWidget* parent)
    : QWidget(parent),
    currentUser(user),
    controller(new DealController(this)),
    selectedDealId(-1)
{
    setupUi();
    configureAccessByRole();
    loadDeals();
}

void DealsWindow::setupUi()
{
    setWindowTitle("Сделки");
    resize(850, 550);

    table = new QTableWidget(this);
    table->setColumnCount(6);
    table->setHorizontalHeaderLabels({"ID", "Покупатель", "Товар", "Количество", "Сумма", "Статус"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    customerNameEdit = new QLineEdit(this);
    productNameEdit = new QLineEdit(this);

    quantitySpinBox = new QSpinBox(this);
    quantitySpinBox->setMinimum(1);
    quantitySpinBox->setMaximum(1000000);

    totalAmountSpinBox = new QDoubleSpinBox(this);
    totalAmountSpinBox->setMinimum(0.01);
    totalAmountSpinBox->setMaximum(100000000);
    totalAmountSpinBox->setDecimals(2);

    statusComboBox = new QComboBox(this);
    statusComboBox->addItem("Новая");
    statusComboBox->addItem("Оформлена");
    statusComboBox->addItem("Закрыта");
    statusComboBox->addItem("Отменена");

    addButton = new QPushButton("Добавить", this);
    updateButton = new QPushButton("Изменить", this);
    deleteButton = new QPushButton("Удалить", this);

    messageLabel = new QLabel(this);
    messageLabel->setStyleSheet("color: red;");

    QFormLayout* formLayout = new QFormLayout();
    formLayout->addRow("Покупатель:", customerNameEdit);
    formLayout->addRow("Товар:", productNameEdit);
    formLayout->addRow("Количество:", quantitySpinBox);
    formLayout->addRow("Сумма:", totalAmountSpinBox);
    formLayout->addRow("Статус:", statusComboBox);

    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(addButton);
    buttonsLayout->addWidget(updateButton);
    buttonsLayout->addWidget(deleteButton);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(table);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonsLayout);
    mainLayout->addWidget(messageLabel);

    setLayout(mainLayout);

    connect(addButton, &QPushButton::clicked,
            this, &DealsWindow::onAddClicked);

    connect(updateButton, &QPushButton::clicked,
            this, &DealsWindow::onUpdateClicked);

    connect(deleteButton, &QPushButton::clicked,
            this, &DealsWindow::onDeleteClicked);

    connect(table, &QTableWidget::cellClicked,
            this, &DealsWindow::onTableRowClicked);
}

void DealsWindow::configureAccessByRole()
{
    if (!currentUser.isAdmin()) {
        updateButton->hide();
        deleteButton->hide();

        statusComboBox->setEnabled(false);
    }
}

void DealsWindow::loadDeals()
{
    QVector<Deal> deals = controller->getDeals();

    table->setRowCount(deals.size());

    for (int i = 0; i < deals.size(); ++i) {
        const Deal& deal = deals[i];

        table->setItem(i, 0, new QTableWidgetItem(QString::number(deal.id())));
        table->setItem(i, 1, new QTableWidgetItem(deal.customerName()));
        table->setItem(i, 2, new QTableWidgetItem(deal.productName()));
        table->setItem(i, 3, new QTableWidgetItem(QString::number(deal.quantity())));
        table->setItem(i, 4, new QTableWidgetItem(QString::number(deal.totalAmount(), 'f', 2)));
        table->setItem(i, 5, new QTableWidgetItem(deal.status()));
    }
}

void DealsWindow::onAddClicked()
{
    QString status = currentUser.isAdmin()
    ? statusComboBox->currentText()
    : "Новая";

    bool success = controller->addDeal(
        customerNameEdit->text(),
        productNameEdit->text(),
        quantitySpinBox->value(),
        totalAmountSpinBox->value(),
        status
        );

    if (!success) {
        messageLabel->setText("Ошибка: проверьте данные сделки");
        return;
    }

    messageLabel->setText("");
    clearForm();
    loadDeals();
}

void DealsWindow::onUpdateClicked()
{
    if (selectedDealId == -1) {
        messageLabel->setText("Выберите сделку для изменения");
        return;
    }

    bool success = controller->updateDeal(
        selectedDealId,
        customerNameEdit->text(),
        productNameEdit->text(),
        quantitySpinBox->value(),
        totalAmountSpinBox->value(),
        statusComboBox->currentText()
        );

    if (!success) {
        messageLabel->setText("Ошибка: сделка не изменена");
        return;
    }

    messageLabel->setText("");
    clearForm();
    loadDeals();
}

void DealsWindow::onDeleteClicked()
{
    if (selectedDealId == -1) {
        messageLabel->setText("Выберите сделку для удаления");
        return;
    }

    bool success = controller->deleteDeal(selectedDealId);

    if (!success) {
        messageLabel->setText("Ошибка: сделка не удалена");
        return;
    }

    messageLabel->setText("");
    clearForm();
    loadDeals();
}

void DealsWindow::onTableRowClicked(int row, int column)
{
    Q_UNUSED(column);

    selectedDealId = table->item(row, 0)->text().toInt();

    customerNameEdit->setText(table->item(row, 1)->text());
    productNameEdit->setText(table->item(row, 2)->text());
    quantitySpinBox->setValue(table->item(row, 3)->text().toInt());
    totalAmountSpinBox->setValue(table->item(row, 4)->text().toDouble());

    QString status = table->item(row, 5)->text();
    int index = statusComboBox->findText(status);

    if (index >= 0) {
        statusComboBox->setCurrentIndex(index);
    }
}

void DealsWindow::clearForm()
{
    selectedDealId = -1;
    customerNameEdit->clear();
    productNameEdit->clear();
    quantitySpinBox->setValue(1);
    totalAmountSpinBox->setValue(0.01);
    statusComboBox->setCurrentIndex(0);
    table->clearSelection();
}
