#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "../controllers/AuthController.h"
#include "../models/User.h"
#include "registerwindow.h"

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget* parent = nullptr);

signals:
    void loginSuccess(const User& user);

private slots:
    void onLoginClicked();
    void onRegisterClicked();

private:
    QLineEdit* loginEdit;
    QLineEdit* passwordEdit;
    QPushButton* loginButton;
    QLabel* errorLabel;
    QPushButton* registerButton;

    AuthController* authController;

    RegisterWindow* registerWindow;
};

#endif // LOGINWINDOW_H
