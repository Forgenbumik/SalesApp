#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QSqlDatabase>

class DatabaseManager
{
public:
    static DatabaseManager& instance();

    bool connectFromConfig(const QString& filePath = "database.ini");

    bool connectToDatabase(const QString& host,
                           int port,
                           const QString& databaseName,
                           const QString& userName,
                           const QString& password);

    QSqlDatabase database() const;

    bool isOpen() const;
    QString lastError() const;

private:
    DatabaseManager();

    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    QSqlDatabase db;
    QString errorText;
};

#endif // DATABASEMANAGER_H
