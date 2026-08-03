#ifndef DEALSWINDOW_H
#define DEALSWINDOW_H

#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>
#include <QVector>
#include <QWidget>

#include "../controllers/dealcontroller.h"
#include "../models/customer.h"
#include "../models/deal.h"
#include "../models/discountrule.h"
#include "../models/product.h"
#include "../models/user.h"

class DealsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DealsWindow(
        const User& currentUser,
        QWidget* parent = nullptr
        );

private slots:
    void onDealRowClicked(int row, int column);
    void onItemRowClicked(int row, int column);

    void onProductChanged(int index);
    void onQuantityChanged(int quantity);
    void onDiscountChanged(int index);

    void onAddItemClicked();
    void onUpdateItemClicked();
    void onRemoveItemClicked();

    void onCreateDealClicked();
    void onUpdateDealClicked();
    void onDeleteDealClicked();

    void onNewDealClicked();
    void onRefreshClicked();

private:
    User m_currentUser;
    DealController* m_controller;

    QVector<Customer> m_customers;
    QVector<Product> m_products;
    QVector<DiscountRule> m_discountRules;
    QVector<DealItem> m_items;

    int m_selectedDealId;
    int m_selectedItemRow;

    QTableWidget* m_dealsTable;
    QTableWidget* m_itemsTable;

    QComboBox* m_customerComboBox;
    QComboBox* m_discountComboBox;
    QComboBox* m_saleTypeComboBox;
    QComboBox* m_statusComboBox;
    QComboBox* m_productComboBox;

    QSpinBox* m_quantitySpinBox;

    QLabel* m_productPriceLabel;
    QLabel* m_productStockLabel;
    QLabel* m_lineTotalLabel;

    QLabel* m_subtotalLabel;
    QLabel* m_discountPercentLabel;
    QLabel* m_discountAmountLabel;
    QLabel* m_totalAmountLabel;
    QLabel* m_discountHintLabel;
    QLabel* m_messageLabel;

    QPushButton* m_addItemButton;
    QPushButton* m_updateItemButton;
    QPushButton* m_removeItemButton;

    QPushButton* m_createDealButton;
    QPushButton* m_updateDealButton;
    QPushButton* m_deleteDealButton;

    QPushButton* m_newDealButton;
    QPushButton* m_refreshButton;

    void setupUi();

    void loadReferenceData();
    void loadDeals();

    void populateItemsTable();
    void clearDealForm();

    void updateProductInformation();
    void recalculateTotals();
    void updateUiMode();

    Deal buildDealFromForm() const;

    const Product* findProduct(int productId) const;
    const DiscountRule* findDiscountRule(int ruleId) const;

    void showError(const QString& message);
    void clearMessage();
};

#endif // DEALSWINDOW_H
