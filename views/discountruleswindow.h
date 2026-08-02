#ifndef DISCOUNTRULESWINDOW_H
#define DISCOUNTRULESWINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QLabel>

#include "../controllers/discountrulecontroller.h"

class DiscountRulesWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DiscountRulesWindow(QWidget* parent = nullptr);

private slots:
    void onAddClicked();
    void onUpdateClicked();
    void onDeleteClicked();
    void onTableRowClicked(int row, int column);

private:
    DiscountRuleController* controller;

    QTableWidget* table;

    QLineEdit* nameEdit;
    QDoubleSpinBox* minAmountSpinBox;
    QDoubleSpinBox* discountPercentSpinBox;

    QPushButton* addButton;
    QPushButton* updateButton;
    QPushButton* deleteButton;

    QLabel* messageLabel;

    int selectedRuleId;

    void setupUi();
    void loadRules();
    void clearForm();
};

#endif // DISCOUNTRULESWINDOW_H
