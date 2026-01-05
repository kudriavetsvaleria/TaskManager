#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>

struct TaskData {
    int id;
    QString description;
    bool isCompleted;
    QString instructions;
};

class DatabaseManager
{
public:
    static DatabaseManager& instance();

    bool openDatabase();
    void closeDatabase();

    bool createTask(const QString &description, int &id);
    bool removeTask(int id);
    bool updateTaskStatus(int id, bool isCompleted);
    bool updateTaskDescription(int id, const QString &description);
    bool updateTaskInstructions(int id, const QString &instructions);
    QString getTaskInstructions(int id);
    QList<TaskData> getAllTasks();

private:
    DatabaseManager();
    ~DatabaseManager();
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    bool createTable();

    QSqlDatabase m_db;
};

#endif // DATABASEMANAGER_H
