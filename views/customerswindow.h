#ifndef CUSTOMERSWINDOW_H
#define CUSTOMERSWINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

#include "../controllers/customercontroller.h"
#include "../models/user.h"

class CustomersWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CustomersWindow(const User& user, QWidget* parent = nullptr);

private slots:
    void onAddClicked();
    void onUpdateClicked();
    void onDeleteClicked();
    void onTableRowClicked(int row, int column);

private:
    User currentUser;
    CustomerController* controller;

    QTableWidget* table;

    QLineEdit* fullNameEdit;
    QLineEdit* phoneEdit;
    QLineEdit* emailEdit;

    QPushButton* addButton;
    QPushButton* updateButton;
    QPushButton* deleteButton;

    QLabel* messageLabel;

    int selectedCustomerId;

    void setupUi();
    void loadCustomers();
    void clearForm();
    void configureAccessByRole();
};
#endif // CUSTOMERSWINDOW_H
