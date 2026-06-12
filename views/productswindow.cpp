#include "productswindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QHeaderView>

ProductsWindow::ProductsWindow(const User& user, QWidget* parent)
    : QWidget(parent),
    currentUser(user),
    controller(new ProductController(this)),
    selectedProductId(-1)
{
    setupUi();
    configureAccessByRole();
    loadProducts();
}

void ProductsWindow::setupUi()
{
    setWindowTitle("Товары");
    resize(700, 500);

    table = new QTableWidget(this);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"ID", "Название", "Цена", "Количество"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    nameEdit = new QLineEdit(this);

    priceSpinBox = new QDoubleSpinBox(this);
    priceSpinBox->setMinimum(0);
    priceSpinBox->setMaximum(100000000);
    priceSpinBox->setDecimals(2);

    quantitySpinBox = new QSpinBox(this);
    quantitySpinBox->setMinimum(0);
    quantitySpinBox->setMaximum(1000000);

    addButton = new QPushButton("Добавить", this);
    updateButton = new QPushButton("Изменить", this);
    deleteButton = new QPushButton("Удалить", this);

    messageLabel = new QLabel(this);
    messageLabel->setStyleSheet("color: red;");

    QFormLayout* formLayout = new QFormLayout();
    formLayout->addRow("Название:", nameEdit);
    formLayout->addRow("Цена:", priceSpinBox);
    formLayout->addRow("Количество:", quantitySpinBox);

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
            this, &ProductsWindow::onAddClicked);

    connect(updateButton, &QPushButton::clicked,
            this, &ProductsWindow::onUpdateClicked);

    connect(deleteButton, &QPushButton::clicked,
            this, &ProductsWindow::onDeleteClicked);

    connect(table, &QTableWidget::cellClicked,
            this, &ProductsWindow::onTableRowClicked);
}

void ProductsWindow::configureAccessByRole()
{
    if (!currentUser.isAdmin()) {
        nameEdit->setEnabled(false);
        priceSpinBox->setEnabled(false);
        quantitySpinBox->setEnabled(false);

        addButton->hide();
        updateButton->hide();
        deleteButton->hide();
    }
}

void ProductsWindow::loadProducts()
{
    QVector<Product> products = controller->getProducts();

    table->setRowCount(products.size());

    for (int i = 0; i < products.size(); ++i) {
        const Product& product = products[i];

        table->setItem(i, 0, new QTableWidgetItem(QString::number(product.id())));
        table->setItem(i, 1, new QTableWidgetItem(product.name()));
        table->setItem(i, 2, new QTableWidgetItem(QString::number(product.price(), 'f', 2)));
        table->setItem(i, 3, new QTableWidgetItem(QString::number(product.quantity())));
    }
}

void ProductsWindow::onAddClicked()
{
    bool success = controller->addProduct(
        nameEdit->text(),
        priceSpinBox->value(),
        quantitySpinBox->value()
        );

    if (!success) {
        messageLabel->setText("Ошибка: проверьте данные товара");
        return;
    }

    messageLabel->setText("");
    clearForm();
    loadProducts();
}

void ProductsWindow::onUpdateClicked()
{
    if (selectedProductId == -1) {
        messageLabel->setText("Выберите товар для изменения");
        return;
    }

    bool success = controller->updateProduct(
        selectedProductId,
        nameEdit->text(),
        priceSpinBox->value(),
        quantitySpinBox->value()
        );

    if (!success) {
        messageLabel->setText("Ошибка: товар не изменён");
        return;
    }

    messageLabel->setText("");
    clearForm();
    loadProducts();
}

void ProductsWindow::onDeleteClicked()
{
    if (selectedProductId == -1) {
        messageLabel->setText("Выберите товар для удаления");
        return;
    }

    bool success = controller->deleteProduct(selectedProductId);

    if (!success) {
        messageLabel->setText("Ошибка: товар не удалён");
        return;
    }

    messageLabel->setText("");
    clearForm();
    loadProducts();
}

void ProductsWindow::onTableRowClicked(int row, int column)
{
    Q_UNUSED(column);

    selectedProductId = table->item(row, 0)->text().toInt();

    nameEdit->setText(table->item(row, 1)->text());
    priceSpinBox->setValue(table->item(row, 2)->text().toDouble());
    quantitySpinBox->setValue(table->item(row, 3)->text().toInt());
}

void ProductsWindow::clearForm()
{
    selectedProductId = -1;
    nameEdit->clear();
    priceSpinBox->setValue(0);
    quantitySpinBox->setValue(0);
    table->clearSelection();
}
