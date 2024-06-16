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
}

int num = 0;

List::~List()
{
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

void List::on_addButton_clicked()
{
    QIcon more(":/image/more.png");
    QIcon scratch(":/image/scratch.png");

    QFont fontCenturyGothic;
    fontCenturyGothic.setFamily("Century Gothic");
    fontCenturyGothic.setPointSize(15);

    QListWidgetItem *newItem = new QListWidgetItem(ui->listWidget);
    QWidget *widget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(widget);

    int taskId = ++num;
    newItem->setData(Qt::UserRole, taskId);

    QLabel *label = new QLabel(QString::number(taskId));
    label->setFont(QFont("Arial", 15));
    label->setIndent(7);

    QLineEdit *lineEdit = new QLineEdit();
    lineEdit->setFont(QFont("Arial", 15));
    lineEdit->setPlaceholderText("Type the description ...");
    lineEdit->setFont(fontCenturyGothic);
    lineEdit->setStyleSheet("QLineEdit { background: transparent; color: white; border: none; }");

    QPushButton *strikeButton = new QPushButton(widget);
    int buttonSize = lineEdit->fontMetrics().height() * 1.6;
    strikeButton->setFixedSize(buttonSize, buttonSize);
    connect(strikeButton, &QPushButton::clicked, this, &List::toggleStrikeOut);
    strikeButton->setIcon(scratch);
    strikeButton->setStyleSheet("QPushButton { background-color: #10192a; color: #FFFFFF; border: none; border-radius: 10px;}");

    QPushButton *canvasButton = new QPushButton(widget);
    canvasButton->setFixedSize(buttonSize, buttonSize);
    connect(canvasButton, &QPushButton::clicked, this, [this, taskId]() { openCanvas(taskId); });
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

    updateTaskNumbers();
}

void List::on_removeButton_clicked()
{
    QListWidgetItem *currentItem = ui->listWidget->currentItem();
    if (currentItem)
    {
        int row = ui->listWidget->row(currentItem);
        delete ui->listWidget->takeItem(row);

        updateTaskNumbers();
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
                    QString taskText = lineEdit->text();
                    if (!taskText.isEmpty())
                    {
                        QFont font = lineEdit->font();
                        font.setStrikeOut(!font.strikeOut());
                        lineEdit->setFont(font);
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

    // Переопределяем paintEvent для закругленных углов
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

    // Устанавливаем закругленные углы для диалогового окна
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
    this->instructions[taskId] = instructions;
}

QString List::loadInstructions(int taskId)
{
    return this->instructions.value(taskId, "");
}
