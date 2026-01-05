#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "List.h"
#include "sidewidget.h"
#include <QPainter>

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
};
#endif // MAINWINDOW_H
