#include "registerwindow.h"

#include <QVBoxLayout>
#include <QFormLayout>

RegisterWindow::RegisterWindow(QWidget* parent)
    : QWidget(parent),
    authController(new AuthController(this))
{
    setupUi();
}

void RegisterWindow::setupUi()
{
    setWindowTitle("Регистрация");
    resize(420, 320);

    loginEdit = new QLineEdit(this);
    passwordEdit = new QLineEdit(this);
    repeatPasswordEdit = new QLineEdit(this);
    fullNameEdit = new QLineEdit(this);
    phoneEdit = new QLineEdit(this);
    emailEdit = new QLineEdit(this);

    passwordEdit->setEchoMode(QLineEdit::Password);
    repeatPasswordEdit->setEchoMode(QLineEdit::Password);

    registerButton = new QPushButton("Зарегистрироваться", this);

    messageLabel = new QLabel(this);
    messageLabel->setStyleSheet("color: red;");
    messageLabel->setWordWrap(true);

    QFormLayout* formLayout = new QFormLayout();
    formLayout->addRow("Логин:", loginEdit);
    formLayout->addRow("Пароль:", passwordEdit);
    formLayout->addRow("Повтор пароля:", repeatPasswordEdit);
    formLayout->addRow("ФИО:", fullNameEdit);
    formLayout->addRow("Телефон:", phoneEdit);
    formLayout->addRow("Email:", emailEdit);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(registerButton);
    mainLayout->addWidget(messageLabel);

    setLayout(mainLayout);

    connect(registerButton, &QPushButton::clicked,
            this, &RegisterWindow::onRegisterClicked);
}

void RegisterWindow::onRegisterClicked()
{
    QString errorMessage;

    bool success = authController->registerUser(
        loginEdit->text(),
        passwordEdit->text(),
        repeatPasswordEdit->text(),
        fullNameEdit->text(),
        phoneEdit->text(),
        emailEdit->text(),
        errorMessage
        );

    if (!success) {
        messageLabel->setStyleSheet("color: red;");
        messageLabel->setText(errorMessage);
        return;
    }

    messageLabel->setStyleSheet("color: green;");
    messageLabel->setText("Регистрация выполнена успешно. Теперь можно войти.");

    clearForm();

    emit registrationSuccess();
}

void RegisterWindow::clearForm()
{
    loginEdit->clear();
    passwordEdit->clear();
    repeatPasswordEdit->clear();
    fullNameEdit->clear();
    phoneEdit->clear();
    emailEdit->clear();
}
