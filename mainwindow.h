#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "List.h"
#include "sidewidget.h"
#include <QPainter>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void on_open_clicked();
private:
    Ui::MainWindow *ui;
    List *list;
    SideWidget *sideWidget;
    int countOpen;
    QPoint dragPosition;
    
    // Window dragging
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void moveEvent(QMoveEvent *event) override;
    
    // Smooth movement coordinator
    void moveGroup(const QPoint &pos);
    
    
private:
    bool isMovingGroup = false;
    
    // Throttling for 1000Hz mice
    QTimer *smoothTimer;
    QPoint targetMovePos; 
    void processMoveQueue(); // Slot for timer
};
#endif // MAINWINDOW_H
