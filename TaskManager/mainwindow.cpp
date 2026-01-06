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

#ifdef Q_OS_WIN
#include <windows.h>
#endif

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
    // Qt::Tool hides it from the taskbar, making it look like part of Main
    list->setWindowFlags(Qt::FramelessWindowHint | Qt::Window | Qt::Tool);
    
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
    
    // Setup Smooth Throttling Timer (approx 60-120 FPS Cap)
    smoothTimer = new QTimer(this);
    smoothTimer->setInterval(8); // ~120 FPS
    smoothTimer->setSingleShot(true);
    connect(smoothTimer, &QTimer::timeout, this, &MainWindow::processMoveQueue);
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
        // Use moveGroup for smooth sync
        moveGroup(event->globalPosition().toPoint() - dragPosition);
        event->accept();
    }
}

// 1. Update Mouse Move to move Parent (MainWindow) which is the Anchor
// 1. Update Mouse Move to move Parent (MainWindow) which is the Anchor
void MainWindow::moveEvent(QMoveEvent *event) {
    // Sync Logic: Only run if we are NOT in the middle of a manual group move.
    // This allows OS events (snap, maximize, manual dragging if any) to still work,
    // but prevents our own moveGroup() from triggering redundant recursion.
    if (!isMovingGroup && list && list->isVisible()) {
        QPoint targetPos(this->x(), this->y() + this->height() - 15);
        if (list->pos() != targetPos) {
             list->move(targetPos);
             list->syncCanvas();
        }
    }
    QMainWindow::moveEvent(event);
}

// Queue the move request
void MainWindow::moveGroup(const QPoint &mainPos) {
    targetMovePos = mainPos;
    if (!smoothTimer->isActive()) {
        smoothTimer->start();
    }
}

// Execute the actual move (Throttled)
void MainWindow::processMoveQueue() {
    QPoint mainPos = targetMovePos;

    isMovingGroup = true; // Block moveEvent sync logic
    
#ifdef Q_OS_WIN
    // ATOMIC MOVEMENT FOR WINDOWS (Pixel-Perfect Smoothness)
    // We use DeferWindowPos to move all windows in a single screen refresh cycle.
    
    // Calculate 3rd window positions
    int x1 = mainPos.x();
    int y1 = mainPos.y();
    
    // List is below Main
    int x2 = x1;
    int y2 = y1 + this->height() - 15;
    
    // Canvas is to the right of List
    int x3 = x2 + 413 + 10; // 413 is hardcoded list width, or use list->width() if avail
    int y3 = y2;

    int numWindows = 1;
    if (list && list->isVisible()) numWindows++;
    if (list && list->getActiveCanvas() && list->getActiveCanvas()->isVisible()) numWindows++;

    HDWP hDWP = BeginDeferWindowPos(numWindows);
    
    // 1. Queue Main Window
    hDWP = DeferWindowPos(hDWP, (HWND)this->winId(), NULL, x1, y1, 0, 0, 
                          SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
                          
    // 2. Queue List Window
    if (list && list->isVisible()) {
        hDWP = DeferWindowPos(hDWP, (HWND)list->winId(), NULL, x2, y2, 0, 0, 
                              SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
                              
        // 3. Queue Canvas Window
        if (list->getActiveCanvas() && list->getActiveCanvas()->isVisible()) {
            hDWP = DeferWindowPos(hDWP, (HWND)list->getActiveCanvas()->winId(), NULL, x3, y3, 0, 0, 
                                  SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
        }
    }
    
    // Execute all moves atomically
    EndDeferWindowPos(hDWP);
    
#else
    // Fallback for non-Windows (Standard Qt Move)
    
    // 1. Move Main Window
    if (this->pos() != mainPos) {
        this->move(mainPos);
    }
    
    // 2. Explicitly Move List (if visible)
    if (list && list->isVisible()) {
        QPoint listPos(mainPos.x(), mainPos.y() + this->height() - 15);
         if (list->pos() != listPos) {
             list->move(listPos);
        }
        // 3. Explicitly Sync Canvas
        list->syncCanvas();
    }
#endif

    isMovingGroup = false; // Unblock
}

void MainWindow::on_open_clicked() {
    QIcon iconArrow(":/image/arrow.png");
    QIcon iconBackArrow(":/image/backarrow.png");

    if (!list->isVisible()){
        // Position it BELOW before showing
        list->move(this->x(), this->y() + this->height() - 15);
        list->show();
        sideWidget->getOpenButton()->setIcon(iconBackArrow);
    }
    else {
        list->hide();
        sideWidget->getOpenButton()->setIcon(iconArrow);
    }
}

