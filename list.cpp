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
#include <QMouseEvent>
#include <QSettings>
#include "constants.h"

List::List(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::List)
{
    ui->setupUi(this);
    setFixedSize(413, 462);

    this->setObjectName("List");

    QFile styleFile(":/style.css"); // Corrected path
    if (styleFile.open(QFile::ReadOnly)) {
        QString style = styleFile.readAll();
        qApp->setStyleSheet(style); // Apply style
        styleFile.close();
    }

    this->setAttribute(Qt::WA_TranslucentBackground);

    // Initialize Database
    DatabaseManager::instance().openDatabase();
    loadTasksFromDb();
    
    // Style ListWidget to match Canvas Dialog's TextEdit
    ui->listWidget->setStyleSheet(QString("QListWidget { background-color: %1; color: %2; border: none; border-radius: 10px; }").arg(Constants::COLOR_TEXT_EDIT_BG, Constants::COLOR_TEXT_WHITE));
    
    // Explicitly apply style to buttons to override any resource caching issues
    // Using exact string from okayButton (line 306)
    QString btnStyle = "QPushButton { background-color: #10192a; color: #FFFFFF; border: none; border-radius: 10px;}";
    ui->addButton->setStyleSheet(btnStyle);
    ui->removeButton->setStyleSheet(btnStyle);
}



void List::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(this->rect(), 20, 20);
    painter.fillPath(path, QBrush(QColor(Constants::COLOR_BACKGROUND)));
    QWidget::paintEvent(event);
}

void List::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (parentWidget())
             dragPosition = event->globalPosition().toPoint() - parentWidget()->frameGeometry().topLeft();
        else 
             dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
             
        event->accept();
    }
}

// 1. Update Mouse Move to move Parent (MainWindow) which is the Anchor
void List::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        if (parentWidget()) {
            parentWidget()->move(event->globalPosition().toPoint() - dragPosition);
        }
        event->accept();
    }
}

// 2. Propagate movement to Canvas
void List::moveEvent(QMoveEvent *event) {
    syncCanvas();
    QWidget::moveEvent(event);
}

void List::syncCanvas() {
    if (activeCanvas && activeCanvas->isVisible()) {
        QPoint targetPos(this->x() + this->width() + 10, this->y());
        if (activeCanvas->pos() != targetPos) {
             activeCanvas->move(targetPos);
        }
    }
}

// 3. Modeless Canvas with Dragging support
void List::openCanvas(int taskId)
{
    if (activeCanvas) {
        activeCanvas->close();
        delete activeCanvas;
        activeCanvas = nullptr;
    }

    QIcon OK(":/image/OK.png");

    activeCanvas = new QDialog(nullptr); // No parent to prevent clipping
    // Qt::Tool hides from taskbar. 
    activeCanvas->setWindowFlags(Qt::FramelessWindowHint | Qt::Window | Qt::Tool);
    activeCanvas->setFixedSize(413, 460);
    activeCanvas->setAttribute(Qt::WA_TranslucentBackground);
    activeCanvas->setObjectName("canvasDialog");

    // Position it
    int x = this->x() + this->width() + 10;
    int y = this->y();
    activeCanvas->move(x, y);

    QTextEdit *detailsTextEdit = new QTextEdit(activeCanvas);
    detailsTextEdit->setGeometry(20, 20, 371, 341);
    detailsTextEdit->setFont(QFont(Constants::FONT_PRIMARY, 12));
    detailsTextEdit->setStyleSheet(QString("QTextEdit { background-color: %1; color: %2; border: none; border-radius: 10px;}").arg(Constants::COLOR_TEXT_EDIT_BG, Constants::COLOR_TEXT_WHITE));

    QString instructions = loadInstructions(taskId);
    detailsTextEdit->setText(instructions);

    QPushButton *okayButton = new QPushButton(activeCanvas);
    okayButton->setGeometry(20, 380, 371, 61);
    okayButton->setIcon(OK);
    okayButton->setIconSize(QSize(28, 28));
    okayButton->setStyleSheet("QPushButton { background-color: #10192a; color: #FFFFFF; border: none; border-radius: 10px;}");
    
    // Save on click
    connect(okayButton, &QPushButton::clicked, [this, taskId, detailsTextEdit, okayButton]() {
        saveInstructions(taskId, detailsTextEdit->toPlainText());
        if (activeCanvas) {
             activeCanvas->close();
             // We don't delete here immediately to avoid crash in event loop? 
             // close() hides it. We can delete it. 
             // But actually, openCanvas deletes previous one.
             // Let's just hide/close.
        }
    });

    activeCanvas->installEventFilter(this);
    activeCanvas->show(); 
}

// Ensure Canvas is closed when List is hidden
void List::hideEvent(QHideEvent *event) {
    if (activeCanvas) {
        activeCanvas->hide();
    }
    QWidget::hideEvent(event);
}

// Ensure Canvas is shown (if it was active?) - No, usually we want it fresh.
// But we need to declare hideEvent in header.

// Destructor
List::~List()
{
    if (activeCanvas) {
        delete activeCanvas;
    }
    delete ui;
}

bool List::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == activeCanvas)
    {
        if (event->type() == QEvent::Paint) {
            QDialog *canvasDialog = qobject_cast<QDialog*>(obj);
            if (canvasDialog)
            {
                QPainter painter(canvasDialog);
                painter.setRenderHint(QPainter::Antialiasing);
                QPainterPath path;
                path.addRoundedRect(canvasDialog->rect(), Constants::WINDOW_RADIUS, Constants::WINDOW_RADIUS);
                painter.fillPath(path, QBrush(QColor(Constants::COLOR_BACKGROUND)));
            }
            return true;
        }
        else if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                 // Calculate drag pos relative to MAIN WINDOW Anchor
                 // We want to drag the ANCHOR (MainWindow) when we drag the Canvas
                 if (parentWidget()) {
                     // Store offset from Main Window top-left
                     dragPosition = mouseEvent->globalPosition().toPoint() - parentWidget()->frameGeometry().topLeft();
                 }
                 mouseEvent->accept();
                 return true;
            }
        }
        else if (event->type() == QEvent::MouseMove) {
             QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
             if (mouseEvent->buttons() & Qt::LeftButton) {
                 if (parentWidget()) {
                     // Move MAIN WINDOW
                     parentWidget()->move(mouseEvent->globalPosition().toPoint() - dragPosition);
                 }
                 mouseEvent->accept();
                 return true;
             }
        }
    }
    return QWidget::eventFilter(obj, event);
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
    fontCenturyGothic.setFamily(Constants::FONT_PRIMARY);
    fontCenturyGothic.setPointSize(Constants::FONT_SIZE_LARGE);

    QListWidgetItem *newItem = new QListWidgetItem(ui->listWidget);
    QWidget *widget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(widget);

    newItem->setData(Qt::UserRole, id);

    QLabel *label = new QLabel();
    label->setFont(QFont(Constants::FONT_SECONDARY, Constants::FONT_SIZE_LARGE));
    label->setIndent(7);

    QLineEdit *lineEdit = new QLineEdit();
    lineEdit->setFont(QFont(Constants::FONT_SECONDARY, Constants::FONT_SIZE_LARGE));
    lineEdit->setPlaceholderText("Type the description ...");
    lineEdit->setFont(fontCenturyGothic);
    lineEdit->setText(text);
    if (isCompleted) {
         QFont font = lineEdit->font();
         font.setStrikeOut(true);
         lineEdit->setFont(font);
    }
    lineEdit->setStyleSheet("QLineEdit { background: transparent; color: white; border: none; }");
    connect(lineEdit, &QLineEdit::editingFinished, this, [id, lineEdit](){
         DatabaseManager::instance().updateTaskDescription(id, lineEdit->text());
    });
    
    QPushButton *strikeButton = new QPushButton(widget);
    int buttonSize = lineEdit->fontMetrics().height() * 1.6;
    strikeButton->setFixedSize(buttonSize, buttonSize);
    connect(strikeButton, &QPushButton::clicked, this, &List::toggleStrikeOut);
    strikeButton->setIcon(scratch);
    strikeButton->setStyleSheet(QString("QPushButton { background-color: %1; color: %2; border: none; border-radius: 10px;}").arg(Constants::COLOR_ITEM_BACKGROUND, Constants::COLOR_TEXT_WHITE));

    QPushButton *canvasButton = new QPushButton(widget);
    canvasButton->setFixedSize(buttonSize, buttonSize);
    connect(canvasButton, &QPushButton::clicked, this, [this, id]() { openCanvas(id); });
    canvasButton->setIcon(more);
    canvasButton->setStyleSheet(QString("QPushButton { background-color: %1; color: %2; border: none; border-radius: 10px;}").arg(Constants::COLOR_ITEM_BACKGROUND, Constants::COLOR_TEXT_WHITE));

    layout->addWidget(label);
    layout->addWidget(lineEdit);
    layout->addWidget(strikeButton);
    layout->addWidget(canvasButton);

    layout->setSpacing(Constants::LIST_ITEM_SPACING);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(Constants::LIST_ITEM_MARGIN, 4, Constants::LIST_ITEM_MARGIN, 4);
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
    fontCenturyGothic.setFamily(Constants::FONT_PRIMARY);
    fontCenturyGothic.setPointSize(Constants::FONT_SIZE_LARGE);

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
                        bool isComplete = !lineEdit->font().strikeOut(); 
                        
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

void List::saveInstructions(int taskId, const QString &instructions)
{
    DatabaseManager::instance().updateTaskInstructions(taskId, instructions);
}

QString List::loadInstructions(int taskId)
{
     return DatabaseManager::instance().getTaskInstructions(taskId);
}
