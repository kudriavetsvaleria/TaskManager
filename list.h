
#ifndef LIST_H
#define LIST_H

#include <QWidget>
#include <QMainWindow>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDialog>
#include <QTextEdit>
#include <QMap>
#include <QPainter>
#include <QPainterPath>

#include "databasemanager.h"

namespace Ui {
class List;
}

class List : public QWidget
{
    Q_OBJECT

public:
    explicit List(QWidget *parent = nullptr);
    ~List();

protected:
    void paintEvent(QPaintEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override; 
    
    // Window dragging
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;


private slots:
    void on_addButton_clicked();
    void on_removeButton_clicked();
    void updateTaskNumbers();
    void toggleStrikeOut();
    void openCanvas(int taskId); // Изменено: добавлен параметр taskId
    void saveInstructions(int taskId, const QString &instructions);
    QString loadInstructions(int taskId);


private:
    Ui::List *ui;
    // QMap<int, QString> instructions; // Removed in favor of DB
    void loadTasksFromDb();
    void createTaskItem(int id, const QString &text, bool isCompleted);
    QPoint dragPosition;
};

#endif // LIST_H
