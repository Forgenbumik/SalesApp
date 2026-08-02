#include "databasemanager.h"

#include <QFileInfo>
#include <QSettings>
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

bool DatabaseManager::connectFromConfig(const QString& filePath)
{
    QFileInfo configFile(filePath);

    if (!configFile.exists()) {
        errorText = "Не найден файл конфигурации базы данных: "
                    + configFile.absoluteFilePath();
        return false;
    }

    QSettings settings(filePath, QSettings::IniFormat);

    const QString host =
        settings.value("database/host", "localhost").toString();

    const int port =
        settings.value("database/port", 5432).toInt();

    const QString databaseName =
        settings.value("database/name").toString();

    const QString userName =
        settings.value("database/user").toString();

    const QString password =
        settings.value("database/password").toString();

    if (databaseName.isEmpty() || userName.isEmpty()) {
        errorText = "В database.ini не заполнены имя базы данных или пользователь";
        return false;
    }

    return connectToDatabase(
        host,
        port,
        databaseName,
        userName,
        password
        );
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
