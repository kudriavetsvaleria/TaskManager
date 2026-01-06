#include "databasemanager.h"

DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

DatabaseManager::DatabaseManager()
{
}

DatabaseManager::~DatabaseManager()
{
    closeDatabase();
}

bool DatabaseManager::openDatabase()
{
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(appDataPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QString dbPath = appDataPath + "/tasks.db";
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbPath);

    if (!m_db.open()) {
        qDebug() << "Error: connection with database failed";
        return false;
    } else {
        qDebug() << "Database: connection ok";
    }

    return createTable();
}

void DatabaseManager::closeDatabase()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
}

bool DatabaseManager::createTable()
{
    QSqlQuery query;
    QString createTableQuery = "CREATE TABLE IF NOT EXISTS tasks ("
                               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                               "description TEXT, "
                               "is_completed INTEGER, "
                               "instructions TEXT)";
    
    if (!query.exec(createTableQuery)) {
        qDebug() << "Database: error creating table" << query.lastError();
        return false;
    }
    
    return true;
}

bool DatabaseManager::createTask(const QString &description, int &id)
{
    QSqlQuery query;
    query.prepare("INSERT INTO tasks (description, is_completed, instructions) VALUES (:desc, 0, '')");
    query.bindValue(":desc", description);

    if (query.exec()) {
        id = query.lastInsertId().toInt();
        return true;
    } else {
        qDebug() << "Database: error create task" << query.lastError();
        return false;
    }
}

bool DatabaseManager::removeTask(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM tasks WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Database: error delete task" << query.lastError();
        return false;
    }
}

bool DatabaseManager::updateTaskStatus(int id, bool isCompleted)
{
    QSqlQuery query;
    query.prepare("UPDATE tasks SET is_completed = :completed WHERE id = :id");
    query.bindValue(":completed", isCompleted ? 1 : 0);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Database: error update status" << query.lastError();
        return false;
    }
    return true;
}

bool DatabaseManager::updateTaskDescription(int id, const QString &description)
{
    QSqlQuery query;
    query.prepare("UPDATE tasks SET description = :desc WHERE id = :id");
    query.bindValue(":desc", description);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Database: error update description" << query.lastError();
        return false;
    }
    return true;
}

bool DatabaseManager::updateTaskInstructions(int id, const QString &instructions)
{
    QSqlQuery query;
    query.prepare("UPDATE tasks SET instructions = :instr WHERE id = :id");
    query.bindValue(":instr", instructions);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Database: error update instructions" << query.lastError();
        return false;
    }
    return true;
}

QString DatabaseManager::getTaskInstructions(int id)
{
    QSqlQuery query;
    query.prepare("SELECT instructions FROM tasks WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }
    return "";
}

QList<TaskData> DatabaseManager::getAllTasks()
{
    QList<TaskData> tasks;
    QSqlQuery query("SELECT id, description, is_completed, instructions FROM tasks");

    while (query.next()) {
        TaskData task;
        task.id = query.value(0).toInt();
        task.description = query.value(1).toString();
        task.isCompleted = query.value(2).toBool();
        task.instructions = query.value(3).toString();
        tasks.append(task);
    }
    return tasks;
}
