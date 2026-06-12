#ifndef PRODUCTSWINDOW_H
#define PRODUCTSWINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QLabel>

#include "../controllers/productcontroller.h"
#include "../models/user.h"

class ProductsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ProductsWindow(const User& user, QWidget* parent = nullptr);

private slots:
    void onAddClicked();
    void onUpdateClicked();
    void onDeleteClicked();
    void onTableRowClicked(int row, int column);

private:
    User currentUser;
    ProductController* controller;

    QTableWidget* table;

    QLineEdit* nameEdit;
    QDoubleSpinBox* priceSpinBox;
    QSpinBox* quantitySpinBox;

    QPushButton* addButton;
    QPushButton* updateButton;
    QPushButton* deleteButton;

    QLabel* messageLabel;

    int selectedProductId;

    void setupUi();
    void loadProducts();
    void clearForm();
    void configureAccessByRole();
};

#endif // PRODUCTSWINDOW_H
