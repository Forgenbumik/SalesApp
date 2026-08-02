#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include "../models/user.h"
#include "productswindow.h"
#include "customerswindow.h"
#include "dealswindow.h"
#include "profilewindow.h"
#include "discountruleswindow.h"
#include "../database/databasemanager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const User& user, QWidget* parent = nullptr);

private:
    User currentUser;

    QWidget* centralWidget;
    QVBoxLayout* mainLayout;

    QLabel* titleLabel;
    QLabel* roleLabel;

    QPushButton* productsButton;
    QPushButton* customersButton;
    QPushButton* dealsButton;
    QPushButton* discountsButton;
    QPushButton* profileButton;
    QPushButton* dbTestButton;

    ProductsWindow* productsWindow;
    CustomersWindow* customersWindow;
    DealsWindow* dealsWindow;
    ProfileWindow* profileWindow;
    DiscountRulesWindow* discountRulesWindow;
private slots:
    void openProductsWindow();
    void openCustomersWindow();
    void openDealsWindow();
    void openProfileWindow();
    void updateCurrentUser(const User& user);
    void testDatabaseConnection();
    void openDiscountRulesWindow();
};

#endif // MAINWINDOW_H
