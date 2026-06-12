#include "loginwindow.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>

LoginWindow::LoginWindow(QWidget* parent)
    : QWidget(parent),
    registerWindow(nullptr)
{
    setWindowTitle("Авторизация");
    resize(350, 220);

    authController = new AuthController(this);

    loginEdit = new QLineEdit(this);
    passwordEdit = new QLineEdit(this);

    loginButton = new QPushButton("Войти", this);
    registerButton = new QPushButton("Регистрация", this);

    errorLabel = new QLabel(this);

    passwordEdit->setEchoMode(QLineEdit::Password);

    errorLabel->setStyleSheet("color: red;");
    errorLabel->setText("");

    QFormLayout* formLayout = new QFormLayout();
    formLayout->addRow("Логин:", loginEdit);
    formLayout->addRow("Пароль:", passwordEdit);

    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(loginButton);
    buttonsLayout->addWidget(registerButton);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonsLayout);
    mainLayout->addWidget(errorLabel);

    setLayout(mainLayout);

    connect(loginButton, &QPushButton::clicked,
            this, &LoginWindow::onLoginClicked);

    connect(registerButton, &QPushButton::clicked,
            this, &LoginWindow::onRegisterClicked);
}

void LoginWindow::onLoginClicked()
{
    QString login = loginEdit->text();
    QString password = passwordEdit->text();

    User user;

    if (authController->login(login, password, user)) {
        errorLabel->setText("");
        emit loginSuccess(user);
    } else {
        errorLabel->setText("Неверный логин или пароль");
    }
}

void LoginWindow::onRegisterClicked()
{
    if (registerWindow == nullptr) {
        registerWindow = new RegisterWindow();
    }

    registerWindow->show();
    registerWindow->raise();
    registerWindow->activateWindow();
}
