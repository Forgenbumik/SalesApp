#include "mainwindow.h"

MainWindow::MainWindow(const User& user, QWidget* parent)
    : QMainWindow(parent),
    currentUser(user),
    productsWindow(nullptr),
    customersWindow(nullptr),
    dealsWindow(nullptr),
    profileWindow(nullptr)
{
    setWindowTitle("Система обработки сделок");
    resize(600, 400);

    centralWidget = new QWidget(this);
    mainLayout = new QVBoxLayout(centralWidget);

    titleLabel = new QLabel("Главное меню", this);
    roleLabel = new QLabel("Пользователь: " + currentUser.login()
                               + " | Роль: " + currentUser.role(), this);

    profileButton = new QPushButton("Профиль", this);

    productsButton = new QPushButton("Товары", this);
    customersButton = new QPushButton("Покупатели", this);
    dealsButton = new QPushButton("Сделки", this);
    usersButton = new QPushButton("Пользователи и роли", this);
    discountsButton = new QPushButton("Правила скидок", this);

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(roleLabel);
    mainLayout->addSpacing(20);

    mainLayout->addWidget(profileButton);

    mainLayout->addWidget(productsButton);
    mainLayout->addWidget(customersButton);
    mainLayout->addWidget(dealsButton);

    if (currentUser.isAdmin()) {
        mainLayout->addWidget(usersButton);
        mainLayout->addWidget(discountsButton);
    } else {
        usersButton->hide();
        discountsButton->hide();
    }

    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    connect(productsButton, &QPushButton::clicked,
            this, &MainWindow::openProductsWindow);
    connect(customersButton, &QPushButton::clicked,
            this, &MainWindow::openCustomersWindow);
    connect(dealsButton, &QPushButton::clicked,
            this, &MainWindow::openDealsWindow);
    connect(profileButton, &QPushButton::clicked,
            this, &MainWindow::openProfileWindow);
}

void MainWindow::openProductsWindow()
{
    if (productsWindow == nullptr) {
        productsWindow = new ProductsWindow(currentUser);
    }

    productsWindow->show();
    productsWindow->raise();
    productsWindow->activateWindow();
}

void MainWindow::openCustomersWindow()
{
    if (customersWindow == nullptr) {
        customersWindow = new CustomersWindow(currentUser);
    }

    customersWindow->show();
    customersWindow->raise();
    customersWindow->activateWindow();
}

void MainWindow::openDealsWindow()
{
    if (dealsWindow == nullptr) {
        dealsWindow = new DealsWindow(currentUser);
    }

    dealsWindow->show();
    dealsWindow->raise();
    dealsWindow->activateWindow();
}

void MainWindow::openProfileWindow()
{
    if (profileWindow == nullptr) {
        profileWindow = new ProfileWindow(currentUser);

        connect(profileWindow, &ProfileWindow::profileUpdated,
                this, &MainWindow::updateCurrentUser);
    }

    profileWindow->show();
    profileWindow->raise();
    profileWindow->activateWindow();
}

void MainWindow::updateCurrentUser(const User& user)
{
    currentUser = user;

    roleLabel->setText("Пользователь: " + currentUser.login()
                       + " | Роль: " + currentUser.role());
}
