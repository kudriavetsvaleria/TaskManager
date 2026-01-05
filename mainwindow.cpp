#include "mainwindow.h"
#include "build/Desktop_Qt_6_7_0_MinGW_64_bit-Debug/ui_mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QFile>
#include <QPainter>
#include <QIcon>
#include <QMouseEvent>
#include <QSettings>
#include <QHBoxLayout>
#include <QRegion>
#include "list.h"
#include "sidewidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , countOpen(0)
{
    ui->setupUi(this);
    
    // Create SideWidget as Central Widget
    sideWidget = new SideWidget(this);
    setCentralWidget(sideWidget); // Direct replacement
    
    // Connect button
    connect(sideWidget->getOpenButton(), &QPushButton::clicked, this, &MainWindow::on_open_clicked);
        
    // Create List as Independent Window but logically linked
    list = new List(this);
    list->setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    
    list->hide();
    
    // Hide old UI elements
    if (ui->open) ui->open->hide();

    // Fix Visibility Issues
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    setAttribute(Qt::WA_TranslucentBackground);
    
    // Restore Position
    QSettings settings("TaskManager", "App");
    if (settings.contains("pos")) {
        move(settings.value("pos").toPoint());
    } else {
        this->move(150, 0); 
    }
    
    // Initial size
    resize(sideWidget->size());
}

MainWindow::~MainWindow()
{
    // Save Position if valid
    if (pos().x() > -1000 && pos().y() > -1000) {
         QSettings settings("TaskManager", "App");
         settings.setValue("pos", pos());
    }

    delete ui; // Children deleted auto
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    // Do nothing. Parents are transparent. Children draw themselves.
    // QWidget::paintEvent(event); 
}

// Window Dragging Implementation - Delegate to children mostly, but if user drags transparency?
// Translucent window passes clicks through empty areas.
// So we rely on SideWidget and List to handle dragging.
void MainWindow::mousePressEvent(QMouseEvent *event) {
     // If we click on transparent area, the OS might handle it or we ignore.
     // But if we want to support dragging from "gap", we can.
     if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPosition().toPoint() - dragPosition);
        event->accept();
    }
}

void MainWindow::moveEvent(QMoveEvent *event) {
    // Sync List position if it's visible
    if (list && list->isVisible()) {
        list->move(this->x() + this->width() + 10, this->y());
    }
    QMainWindow::moveEvent(event);
}

void MainWindow::on_open_clicked() {
    QIcon iconArrow(":/image/arrow.png");
    QIcon iconBackArrow(":/image/backarrow.png");

    if (!list->isVisible()){
        // Position it before showing
        list->move(this->x() + this->width() + 10, this->y());
        list->show();
        sideWidget->getOpenButton()->setIcon(iconBackArrow);
    }
    else {
        list->hide();
        sideWidget->getOpenButton()->setIcon(iconArrow);
    }
}

