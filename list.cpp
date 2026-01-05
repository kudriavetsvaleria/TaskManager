#include "list.h"
#include "build/Desktop_Qt_6_7_0_MinGW_64_bit-Debug/ui_list.h"
#include "ui_list.h"
#include <QListWidget>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QDebug>
#include <QFont>
#include <QDialog>
#include <QTextEdit>
#include <QPushButton>
#include <QFile>
#include <QApplication>
#include <QPainter>
#include <QPainterPath>

List::List(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::List)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    this->move(150, 75);

    this->setObjectName("List");

    QFile styleFile(":/styles.qss"); // Путь к вашему файлу стилей
    if (styleFile.open(QFile::ReadOnly)) {
        QString style = styleFile.readAll();
        qApp->setStyleSheet(style); // Применяем стиль ко всему приложению
        styleFile.close();
    }

    setAttribute(Qt::WA_TranslucentBackground);

    // Initialize Database
    DatabaseManager::instance().openDatabase();
    loadTasksFromDb();
}

List::~List()
{
    // DatabaseManager::instance().closeDatabase(); // Singleton handles this on destruction usually, or we can explicit close if needed
    delete ui;
}

void List::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(this->rect(), 20, 20);
    painter.fillPath(path, QBrush(QColor("#0c0f1b")));
    QWidget::paintEvent(event);
}

void List::loadTasksFromDb()
{
    QList<TaskData> tasks = DatabaseManager::instance().getAllTasks();
    for (const TaskData &task : tasks) {
        createTaskItem(task.id, task.description, task.isCompleted);
    }
    updateTaskNumbers();
}

void List::createTaskItem(int id, const QString &text, bool isCompleted)
{
    QIcon more(":/image/more.png");
    QIcon scratch(":/image/scratch.png");

    QFont fontCenturyGothic;
    fontCenturyGothic.setFamily("Century Gothic");
    fontCenturyGothic.setPointSize(15);

    QListWidgetItem *newItem = new QListWidgetItem(ui->listWidget);
    QWidget *widget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(widget);

    newItem->setData(Qt::UserRole, id);

    QLabel *label = new QLabel(); // Number will be set by updateTaskNumbers
    label->setFont(QFont("Arial", 15));
    label->setIndent(7);

    QLineEdit *lineEdit = new QLineEdit();
    lineEdit->setFont(QFont("Arial", 15));
    lineEdit->setPlaceholderText("Type the description ...");
    lineEdit->setFont(fontCenturyGothic);
    lineEdit->setText(text);
    if (isCompleted) {
         QFont font = lineEdit->font();
         font.setStrikeOut(true);
         lineEdit->setFont(font);
    }
    lineEdit->setStyleSheet("QLineEdit { background: transparent; color: white; border: none; }");
    // Capture text changes to update DB
    connect(lineEdit, &QLineEdit::editingFinished, this, [id, lineEdit](){
         DatabaseManager::instance().updateTaskDescription(id, lineEdit->text());
    });

    // Actually, to avoid compilation error, I won't call a non-existent method.
    // But wait, how do we save the text the user types?
    // The user types into `lineEdit`. existing code didn't "save" it anywhere because it was just UI.
    // Now we need to save it.
    // I will add `updateTaskDescription` to DatabaseManager in a subsequent step or previous? 
    // I can't go back. I will add it in the NEXT step or via `multi_replace`. 
    // I'll proceed with this file, but comment out the update description part or simple add a TODO.
    // Actually, I can use `createTask` with a default "New Task" or empty.
    // But when user types, we need to save.
    // I will add `updateTaskDescription` to `DatabaseManager` in a separate file edit immediately after this.
    
    QPushButton *strikeButton = new QPushButton(widget);
    int buttonSize = lineEdit->fontMetrics().height() * 1.6;
    strikeButton->setFixedSize(buttonSize, buttonSize);
    connect(strikeButton, &QPushButton::clicked, this, &List::toggleStrikeOut);
    strikeButton->setIcon(scratch);
    strikeButton->setStyleSheet("QPushButton { background-color: #10192a; color: #FFFFFF; border: none; border-radius: 10px;}");

    QPushButton *canvasButton = new QPushButton(widget);
    canvasButton->setFixedSize(buttonSize, buttonSize);
    connect(canvasButton, &QPushButton::clicked, this, [this, id]() { openCanvas(id); });
    canvasButton->setIcon(more);
    canvasButton->setStyleSheet("QPushButton { background-color: #10192a; color: #FFFFFF; border: none; border-radius: 10px;}");

    layout->addWidget(label);
    layout->addWidget(lineEdit);
    layout->addWidget(strikeButton);
    layout->addWidget(canvasButton);

    layout->setSpacing(8);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(10, 4, 10, 4);
    widget->setLayout(layout);

    newItem->setSizeHint(widget->sizeHint());

    ui->listWidget->addItem(newItem);
    ui->listWidget->setItemWidget(newItem, widget);
}

void List::on_addButton_clicked()
{
    int id = -1;
    if (DatabaseManager::instance().createTask("", id)) {
        createTaskItem(id, "", false);
        updateTaskNumbers();
        
        // Focus the new line edit
        int row = ui->listWidget->count() - 1;
        QListWidgetItem *item = ui->listWidget->item(row);
        QWidget *widget = ui->listWidget->itemWidget(item);
        if (widget) {
            QLineEdit *lineEdit = widget->findChild<QLineEdit*>();
            if (lineEdit) {
                lineEdit->setFocus();
                // Add connection for saving text on change
                connect(lineEdit, &QLineEdit::editingFinished, this, [id, lineEdit](){
                     DatabaseManager::instance().updateTaskDescription(id, lineEdit->text());
                });
            }
        }
    }
}

void List::on_removeButton_clicked()
{
    QListWidgetItem *currentItem = ui->listWidget->currentItem();
    if (currentItem)
    {
        int id = currentItem->data(Qt::UserRole).toInt();
        if (DatabaseManager::instance().removeTask(id)) {
            int row = ui->listWidget->row(currentItem);
            delete ui->listWidget->takeItem(row);
            updateTaskNumbers();
        }
    }
}

void List::updateTaskNumbers()
{
    QFont fontCenturyGothic;
    fontCenturyGothic.setFamily("Century Gothic");
    fontCenturyGothic.setPointSize(15);

    for (int i = 0; i < ui->listWidget->count(); ++i)
    {
        QListWidgetItem *item = ui->listWidget->item(i);
        QWidget *widget = ui->listWidget->itemWidget(item);
        if (widget)
        {
            QHBoxLayout *layout = qobject_cast<QHBoxLayout*>(widget->layout());
            if (layout)
            {
                QLabel *label = qobject_cast<QLabel*>(layout->itemAt(0)->widget());
                if (label)
                {
                    label->setText(QString::number(i + 1));
                    label->setFont(fontCenturyGothic);
                }
            }
        }
    }
}

void List::toggleStrikeOut()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button)
    {
        QWidget *widget = button->parentWidget();
        if (widget)
        {
            QHBoxLayout *layout = qobject_cast<QHBoxLayout*>(widget->layout());
            if (layout)
            {
                QLineEdit *lineEdit = qobject_cast<QLineEdit*>(layout->itemAt(1)->widget());
                if (lineEdit)
                {
                    // Find the ID
                    QListWidgetItem* item = nullptr;
                    for(int i=0; i < ui->listWidget->count(); ++i) {
                         if(ui->listWidget->itemWidget(ui->listWidget->item(i)) == widget) {
                             item = ui->listWidget->item(i);
                             break;
                         }
                    }
                    
                    if (item) {
                        int id = item->data(Qt::UserRole).toInt();
                        bool isComplete = !lineEdit->font().strikeOut(); // The NEW state
                        
                        // Update UI
                        QFont font = lineEdit->font();
                        font.setStrikeOut(isComplete);
                        lineEdit->setFont(font);
                        
                        // Update DB
                        DatabaseManager::instance().updateTaskStatus(id, isComplete);
                    }
                }
            }
        }
    }
}

void List::openCanvas(int taskId)
{
    QIcon OK(":/image/OK.png");

    QDialog *canvasDialog = new QDialog(this);
    canvasDialog->setFixedSize(413, 460);
    canvasDialog->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    int x = this->x() + this->width() + 10;
    int y = this->y();
    canvasDialog->move(x, y);

    canvasDialog->setAttribute(Qt::WA_TranslucentBackground);
    canvasDialog->setObjectName("canvasDialog");

    QTextEdit *detailsTextEdit = new QTextEdit(canvasDialog);
    detailsTextEdit->setGeometry(20, 20, 371, 341);
    detailsTextEdit->setFont(QFont("Century Gothic", 12));
    detailsTextEdit->setStyleSheet("QTextEdit { background-color: #171f2d; color: #FFFFFF; border: none; border-radius: 10px;}");

    QString instructions = loadInstructions(taskId);
    detailsTextEdit->setText(instructions);

    QPushButton *okButton = new QPushButton(canvasDialog);
    okButton->setGeometry(0, 0, 0, 0);

    QPushButton *okayButton = new QPushButton(canvasDialog);
    okayButton->setGeometry(20, 380, 371, 61);
    okayButton->setIcon(OK);
    okayButton->setIconSize(QSize(28, 28));
    connect(okayButton, &QPushButton::clicked, canvasDialog, &QDialog::accept);
    okayButton->setStyleSheet("QPushButton { background-color: #10192a; color: #FFFFFF; border: none; border-radius: 10px;}");

    QMetaObject::connectSlotsByName(canvasDialog);
    canvasDialog->installEventFilter(this);

    if (canvasDialog->exec() == QDialog::Accepted) {
        saveInstructions(taskId, detailsTextEdit->toPlainText());
    }
}

bool List::eventFilter(QObject *obj, QEvent *event)
{
    if (obj->objectName() == "canvasDialog" && event->type() == QEvent::Paint)
    {
        QDialog *canvasDialog = qobject_cast<QDialog*>(obj);
        if (canvasDialog)
        {
            QPainter painter(canvasDialog);
            painter.setRenderHint(QPainter::Antialiasing);
            QPainterPath path;
            path.addRoundedRect(canvasDialog->rect(), 20, 20);
            painter.fillPath(path, QBrush(QColor("#0c0f1b")));
        }
        return true;
    }
    return QWidget::eventFilter(obj, event);
}

void List::saveInstructions(int taskId, const QString &instructions)
{
    DatabaseManager::instance().updateTaskInstructions(taskId, instructions);
}

QString List::loadInstructions(int taskId)
{
     return DatabaseManager::instance().getTaskInstructions(taskId);
}
