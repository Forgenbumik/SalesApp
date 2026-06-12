#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "../controllers/AuthController.h"
#include "../models/User.h"

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget* parent = nullptr);

signals:
    void loginSuccess(const User& user);

private slots:
    void onLoginClicked();

private:
    QLineEdit* loginEdit;
    QLineEdit* passwordEdit;
    QPushButton* loginButton;
    QLabel* errorLabel;

    AuthController* authController;
};

#endif // LOGINWINDOW_H
