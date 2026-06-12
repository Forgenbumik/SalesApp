#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

#include "../controllers/authcontroller.h"

class RegisterWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterWindow(QWidget* parent = nullptr);

signals:
    void registrationSuccess();

private slots:
    void onRegisterClicked();

private:
    AuthController* authController;

    QLineEdit* loginEdit;
    QLineEdit* passwordEdit;
    QLineEdit* repeatPasswordEdit;
    QLineEdit* fullNameEdit;
    QLineEdit* phoneEdit;
    QLineEdit* emailEdit;

    QPushButton* registerButton;
    QLabel* messageLabel;

    void setupUi();
    void clearForm();
};

#endif // REGISTERWINDOW_H
