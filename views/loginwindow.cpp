#include "loginwindow.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle("Авторизация");
    resize(350, 200);

    authController = new AuthController(this);

    loginEdit = new QLineEdit(this);
    passwordEdit = new QLineEdit(this);
    loginButton = new QPushButton("Войти", this);
    errorLabel = new QLabel(this);

    passwordEdit->setEchoMode(QLineEdit::Password);

    errorLabel->setStyleSheet("color: red;");
    errorLabel->setText("");

    QFormLayout* formLayout = new QFormLayout();
    formLayout->addRow("Логин:", loginEdit);
    formLayout->addRow("Пароль:", passwordEdit);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(loginButton);
    mainLayout->addWidget(errorLabel);

    setLayout(mainLayout);

    connect(loginButton, &QPushButton::clicked,
            this, &LoginWindow::onLoginClicked);
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
