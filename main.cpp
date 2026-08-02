#include "views/mainwindow.h"
#include "views/loginwindow.h"
#include "database/databasemanager.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    bool dbConnected = DatabaseManager::instance().connectFromConfig("database.ini");

    if (!dbConnected) {
        QMessageBox::critical(nullptr,
                              "Ошибка подключения к базе данных",
                              DatabaseManager::instance().lastError());
        return -1;
    }

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
