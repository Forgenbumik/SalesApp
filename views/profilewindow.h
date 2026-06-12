#ifndef PROFILEWINDOW_H
#define PROFILEWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

#include "../models/user.h"
#include "../controllers/authcontroller.h"

class ProfileWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ProfileWindow(const User& user, QWidget* parent = nullptr);

signals:
    void profileUpdated(const User& user);

private slots:
    void onSaveClicked();

private:
    User currentUser;
    AuthController* authController;

    QLabel* loginLabel;
    QLabel* roleLabel;

    QLineEdit* fullNameEdit;
    QLineEdit* phoneEdit;
    QLineEdit* emailEdit;
    QLineEdit* newPasswordEdit;
    QLineEdit* repeatPasswordEdit;

    QPushButton* saveButton;
    QLabel* messageLabel;

    void setupUi();
    void fillForm();
};

#endif // PROFILEWINDOW_H
