#include "discountruleswindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QHeaderView>

DiscountRulesWindow::DiscountRulesWindow(QWidget* parent)
    : QWidget(parent),
    controller(new DiscountRuleController(this)),
    selectedRuleId(-1)
{
    setupUi();
    loadRules();
}

void DiscountRulesWindow::setupUi()
{
    setWindowTitle("Правила скидок");
    resize(750, 500);

    table = new QTableWidget(this);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"ID", "Название", "Мин. сумма", "Скидка, %"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    nameEdit = new QLineEdit(this);

    minAmountSpinBox = new QDoubleSpinBox(this);
    minAmountSpinBox->setMinimum(0);
    minAmountSpinBox->setMaximum(100000000);
    minAmountSpinBox->setDecimals(2);

    discountPercentSpinBox = new QDoubleSpinBox(this);
    discountPercentSpinBox->setMinimum(0.01);
    discountPercentSpinBox->setMaximum(100);
    discountPercentSpinBox->setDecimals(2);

    addButton = new QPushButton("Добавить", this);
    updateButton = new QPushButton("Изменить", this);
    deleteButton = new QPushButton("Удалить", this);

    messageLabel = new QLabel(this);
    messageLabel->setStyleSheet("color: red;");
    messageLabel->setWordWrap(true);

    QFormLayout* formLayout = new QFormLayout();
    formLayout->addRow("Название:", nameEdit);
    formLayout->addRow("Минимальная сумма:", minAmountSpinBox);
    formLayout->addRow("Процент скидки:", discountPercentSpinBox);

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
            this, &DiscountRulesWindow::onAddClicked);

    connect(updateButton, &QPushButton::clicked,
            this, &DiscountRulesWindow::onUpdateClicked);

    connect(deleteButton, &QPushButton::clicked,
            this, &DiscountRulesWindow::onDeleteClicked);

    connect(table, &QTableWidget::cellClicked,
            this, &DiscountRulesWindow::onTableRowClicked);
}

void DiscountRulesWindow::loadRules()
{
    QVector<DiscountRule> rules = controller->getRules();

    table->setRowCount(rules.size());

    for (int i = 0; i < rules.size(); ++i) {
        const DiscountRule& rule = rules[i];

        table->setItem(i, 0, new QTableWidgetItem(QString::number(rule.id())));
        table->setItem(i, 1, new QTableWidgetItem(rule.name()));
        table->setItem(i, 2, new QTableWidgetItem(QString::number(rule.minAmount(), 'f', 2)));
        table->setItem(i, 3, new QTableWidgetItem(QString::number(rule.discountPercent(), 'f', 2)));
    }
}

void DiscountRulesWindow::onAddClicked()
{
    bool success = controller->addRule(
        nameEdit->text(),
        minAmountSpinBox->value(),
        discountPercentSpinBox->value()
        );

    if (!success) {
        messageLabel->setText("Ошибка: проверьте данные правила скидки");
        return;
    }

    messageLabel->setText("");
    clearForm();
    loadRules();
}

void DiscountRulesWindow::onUpdateClicked()
{
    if (selectedRuleId == -1) {
        messageLabel->setText("Выберите правило скидки для изменения");
        return;
    }

    bool success = controller->updateRule(
        selectedRuleId,
        nameEdit->text(),
        minAmountSpinBox->value(),
        discountPercentSpinBox->value()
        );

    if (!success) {
        messageLabel->setText("Ошибка: правило скидки не изменено");
        return;
    }

    messageLabel->setText("");
    clearForm();
    loadRules();
}

void DiscountRulesWindow::onDeleteClicked()
{
    if (selectedRuleId == -1) {
        messageLabel->setText("Выберите правило скидки для удаления");
        return;
    }

    bool success = controller->deleteRule(selectedRuleId);

    if (!success) {
        messageLabel->setText("Ошибка: правило скидки не удалено");
        return;
    }

    messageLabel->setText("");
    clearForm();
    loadRules();
}

void DiscountRulesWindow::onTableRowClicked(int row, int column)
{
    Q_UNUSED(column);

    selectedRuleId = table->item(row, 0)->text().toInt();

    nameEdit->setText(table->item(row, 1)->text());
    minAmountSpinBox->setValue(table->item(row, 2)->text().toDouble());
    discountPercentSpinBox->setValue(table->item(row, 3)->text().toDouble());
}

void DiscountRulesWindow::clearForm()
{
    selectedRuleId = -1;
    nameEdit->clear();
    minAmountSpinBox->setValue(0);
    discountPercentSpinBox->setValue(0.01);
    table->clearSelection();
}
