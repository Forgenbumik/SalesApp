#ifndef DEALSWINDOW_H
#define DEALSWINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QComboBox>
#include <QLabel>

#include "../controllers/dealcontroller.h"
#include "../models/user.h"

class DealsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DealsWindow(const User& user, QWidget* parent = nullptr);

private slots:
    void onAddClicked();
    void onUpdateClicked();
    void onDeleteClicked();
    void onTableRowClicked(int row, int column);

private:
    User currentUser;
    DealController* controller;

    QTableWidget* table;

    QLineEdit* customerNameEdit;
    QLineEdit* productNameEdit;
    QSpinBox* quantitySpinBox;
    QDoubleSpinBox* totalAmountSpinBox;
    QComboBox* statusComboBox;

    QPushButton* addButton;
    QPushButton* updateButton;
    QPushButton* deleteButton;

    QLabel* messageLabel;

    int selectedDealId;

    void setupUi();
    void loadDeals();
    void clearForm();
    void configureAccessByRole();
};

#endif // DEALSWINDOW_H
