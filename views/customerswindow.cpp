#include "customerswindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QHeaderView>

CustomersWindow::CustomersWindow(const User& user, QWidget* parent)
    : QWidget(parent),
    currentUser(user),
    controller(new CustomerController(this)),
    selectedCustomerId(-1)
{
    setupUi();
    configureAccessByRole();
    loadCustomers();
}

void CustomersWindow::setupUi()
{
    setWindowTitle("Покупатели");
    resize(750, 500);

    table = new QTableWidget(this);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"ID", "ФИО", "Телефон", "Email"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    fullNameEdit = new QLineEdit(this);
    phoneEdit = new QLineEdit(this);
    emailEdit = new QLineEdit(this);

    addButton = new QPushButton("Добавить", this);
    updateButton = new QPushButton("Изменить", this);
    deleteButton = new QPushButton("Удалить", this);

    messageLabel = new QLabel(this);
    messageLabel->setStyleSheet("color: red;");

    QFormLayout* formLayout = new QFormLayout();
    formLayout->addRow("ФИО:", fullNameEdit);
    formLayout->addRow("Телефон:", phoneEdit);
    formLayout->addRow("Email:", emailEdit);

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
            this, &CustomersWindow::onAddClicked);

    connect(updateButton, &QPushButton::clicked,
            this, &CustomersWindow::onUpdateClicked);

    connect(deleteButton, &QPushButton::clicked,
            this, &CustomersWindow::onDeleteClicked);

    connect(table, &QTableWidget::cellClicked,
            this, &CustomersWindow::onTableRowClicked);
}

void CustomersWindow::configureAccessByRole()
{
    if (!currentUser.isAdmin()) {
        fullNameEdit->setEnabled(false);
        phoneEdit->setEnabled(false);
        emailEdit->setEnabled(false);

        addButton->hide();
        updateButton->hide();
        deleteButton->hide();
    }
}

void CustomersWindow::loadCustomers()
{
    QVector<Customer> customers = controller->getCustomers();

    table->setRowCount(customers.size());

    for (int i = 0; i < customers.size(); ++i) {
        const Customer& customer = customers[i];

        table->setItem(i, 0, new QTableWidgetItem(QString::number(customer.id())));
        table->setItem(i, 1, new QTableWidgetItem(customer.fullName()));
        table->setItem(i, 2, new QTableWidgetItem(customer.phone()));
        table->setItem(i, 3, new QTableWidgetItem(customer.email()));
    }
}

void CustomersWindow::onAddClicked()
{
    bool success = controller->addCustomer(
        fullNameEdit->text(),
        phoneEdit->text(),
        emailEdit->text()
        );

    if (!success) {
        messageLabel->setText("Ошибка: проверьте ФИО, телефон и email");
        return;
    }

    messageLabel->setText("");
    clearForm();
    loadCustomers();
}

void CustomersWindow::onUpdateClicked()
{
    if (selectedCustomerId == -1) {
        messageLabel->setText("Выберите покупателя для изменения");
        return;
    }

    bool success = controller->updateCustomer(
        selectedCustomerId,
        fullNameEdit->text(),
        phoneEdit->text(),
        emailEdit->text()
        );

    if (!success) {
        messageLabel->setText("Ошибка: покупатель не изменён");
        return;
    }

    messageLabel->setText("");
    clearForm();
    loadCustomers();
}

void CustomersWindow::onDeleteClicked()
{
    if (selectedCustomerId == -1) {
        messageLabel->setText("Выберите покупателя для удаления");
        return;
    }

    bool success = controller->deleteCustomer(selectedCustomerId);

    if (!success) {
        messageLabel->setText("Ошибка: покупатель не удалён");
        return;
    }

    messageLabel->setText("");
    clearForm();
    loadCustomers();
}

void CustomersWindow::onTableRowClicked(int row, int column)
{
    Q_UNUSED(column);

    selectedCustomerId = table->item(row, 0)->text().toInt();

    fullNameEdit->setText(table->item(row, 1)->text());
    phoneEdit->setText(table->item(row, 2)->text());
    emailEdit->setText(table->item(row, 3)->text());
}

void CustomersWindow::clearForm()
{
    selectedCustomerId = -1;
    fullNameEdit->clear();
    phoneEdit->clear();
    emailEdit->clear();
    table->clearSelection();
}
