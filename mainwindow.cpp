#include "mainwindow.h"
#include "build/Desktop_Qt_6_7_0_MinGW_64_bit-Debug/ui_mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QFile>
#include <QPainter>
#include <QIcon>
#include "list.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    this->move(150, 0);
    list = new List;
    list->hide();
    setWindowFlags(Qt::FramelessWindowHint);

    // Установка прозрачного фона для окна
    setAttribute(Qt::WA_TranslucentBackground);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath path;
    QRect rect = this->rect();

    // Создаем прямоугольник без верхних закругленных углов
    path.moveTo(rect.topLeft());
    path.lineTo(rect.topRight());
    path.lineTo(rect.bottomRight());
    path.arcTo(rect.right() - 40, rect.bottom() - 40, 40, 40, 0, -90); // Закругленный нижний правый угол
    path.lineTo(rect.bottomLeft() + QPoint(0, 20)); // Перемещаемся до начала нижнего левого угла, чтобы начать его закругление
    path.arcTo(rect.left(), rect.bottom() - 40, 40, 40, -90, -90); // Закругленный нижний левый угол
    path.lineTo(rect.topLeft());

    painter.fillPath(path, QBrush(QColor("#0c0f1b")));

    // Устанавливаем маску для окна, чтобы оно действительно имело закругленные углы
    QRegion region(path.toFillPolygon().toPolygon());
    this->setMask(region);

    QWidget::paintEvent(event);
}


int countOpen = 0;
void MainWindow::on_open_clicked() {

    QIcon iconArrow(":/image/arrow.png");
    QIcon iconBackArrow(":/image/backarrow.png");

    if (countOpen % 2 == 0){
        list->show();
        ui->open->setIcon(iconBackArrow);
    }
    else {
        list->hide();
        ui->open->setIcon(iconArrow);
    }
    countOpen++;
}

