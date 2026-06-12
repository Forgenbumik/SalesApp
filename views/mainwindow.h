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
    QPushButton* usersButton;
    QPushButton* discountsButton;

    ProductsWindow* productsWindow;
    CustomersWindow* customersWindow;
    DealsWindow* dealsWindow;
private slots:
    void openProductsWindow();
    void openCustomersWindow();
    void openDealsWindow();
};

#endif // MAINWINDOW_H
