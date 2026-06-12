#include "views/mainwindow.h"
#include "views/loginwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    LoginWindow loginWindow;
    loginWindow.show();

    MainWindow* mainWindow = nullptr;

    QObject::connect(&loginWindow, &LoginWindow::loginSuccess,
                     [&](const User& user) {
                         mainWindow = new MainWindow(user);
                         mainWindow->show();
                         loginWindow.close();
                     });

    return app.exec();
}
