#include "databasemanager.h"

#include <QSqlError>

DatabaseManager::DatabaseManager()
{
}

DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager manager;
    return manager;
}

bool DatabaseManager::connectToDatabase(const QString& host,
                                        int port,
                                        const QString& databaseName,
                                        const QString& userName,
                                        const QString& password)
{
    if (QSqlDatabase::contains("main_connection")) {
        db = QSqlDatabase::database("main_connection");
    } else {
        db = QSqlDatabase::addDatabase("QPSQL", "main_connection");
    }

    // Если подключение уже открыто, повторно его не открываем.
    // Это важно: повторное open() может ломать работу QSqlQuery.
    if (db.isValid() && db.isOpen()) {
        errorText.clear();
        return true;
    }

    db.setHostName(host);
    db.setPort(port);
    db.setDatabaseName(databaseName);
    db.setUserName(userName);
    db.setPassword(password);

    if (!db.open()) {
        errorText = db.lastError().text();
        return false;
    }

    errorText.clear();
    return true;
}

QSqlDatabase DatabaseManager::database() const
{
    return db;
}

bool DatabaseManager::isOpen() const
{
    return db.isValid() && db.isOpen();
}

QString DatabaseManager::lastError() const
{
    return errorText;
}
