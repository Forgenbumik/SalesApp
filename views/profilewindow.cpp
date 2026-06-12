#include "profilewindow.h"

#include <QVBoxLayout>
#include <QFormLayout>

ProfileWindow::ProfileWindow(const User& user, QWidget* parent)
    : QWidget(parent),
    currentUser(user),
    authController(new AuthController(this))
{
    setupUi();
    fillForm();
}

void ProfileWindow::setupUi()
{
    setWindowTitle("Профиль пользователя");
    resize(450, 350);

    loginLabel = new QLabel(this);
    roleLabel = new QLabel(this);

    fullNameEdit = new QLineEdit(this);
    phoneEdit = new QLineEdit(this);
    emailEdit = new QLineEdit(this);
    newPasswordEdit = new QLineEdit(this);
    repeatPasswordEdit = new QLineEdit(this);

    newPasswordEdit->setEchoMode(QLineEdit::Password);
    repeatPasswordEdit->setEchoMode(QLineEdit::Password);

    newPasswordEdit->setPlaceholderText("Оставьте пустым, если не хотите менять");
    repeatPasswordEdit->setPlaceholderText("Повторите новый пароль");

    saveButton = new QPushButton("Сохранить изменения", this);

    messageLabel = new QLabel(this);
    messageLabel->setWordWrap(true);
    messageLabel->setStyleSheet("color: red;");

    QFormLayout* formLayout = new QFormLayout();
    formLayout->addRow("Логин:", loginLabel);
    formLayout->addRow("Роль:", roleLabel);
    formLayout->addRow("ФИО:", fullNameEdit);
    formLayout->addRow("Телефон:", phoneEdit);
    formLayout->addRow("Email:", emailEdit);
    formLayout->addRow("Новый пароль:", newPasswordEdit);
    formLayout->addRow("Повтор пароля:", repeatPasswordEdit);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(saveButton);
    mainLayout->addWidget(messageLabel);

    setLayout(mainLayout);

    connect(saveButton, &QPushButton::clicked,
            this, &ProfileWindow::onSaveClicked);
}

void ProfileWindow::fillForm()
{
    loginLabel->setText(currentUser.login());
    roleLabel->setText(currentUser.role());

    fullNameEdit->setText(currentUser.fullName());
    phoneEdit->setText(currentUser.phone());
    emailEdit->setText(currentUser.email());

    newPasswordEdit->clear();
    repeatPasswordEdit->clear();
}

void ProfileWindow::onSaveClicked()
{
    QString errorMessage;
    User updatedUser;

    bool success = authController->updateProfile(
        currentUser.id(),
        fullNameEdit->text(),
        phoneEdit->text(),
        emailEdit->text(),
        newPasswordEdit->text(),
        repeatPasswordEdit->text(),
        updatedUser,
        errorMessage
        );

    if (!success) {
        messageLabel->setStyleSheet("color: red;");
        messageLabel->setText(errorMessage);
        return;
    }

    currentUser = updatedUser;

    messageLabel->setStyleSheet("color: green;");
    messageLabel->setText("Профиль успешно обновлён");

    newPasswordEdit->clear();
    repeatPasswordEdit->clear();

    emit profileUpdated(currentUser);
}
