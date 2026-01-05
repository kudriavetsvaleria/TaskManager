#include "sidewidget.h"
#include "ui_sidewidget.h"
#include "constants.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>

SideWidget::SideWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SideWidget)
{
    ui->setupUi(this);
    setFixedSize(413, 66); // Enforce size
}

SideWidget::~SideWidget()
{
    delete ui;
}

QPushButton* SideWidget::getOpenButton() {
    return ui->open;
}

void SideWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath path;
    QRect rect = this->rect();

    // The logic from original MainWindow::paintEvent
    // Creating rectangle with rounded bottom corners
    path.moveTo(rect.topLeft());
    path.lineTo(rect.topRight());
    path.lineTo(rect.bottomRight());
    path.arcTo(rect.right() - 40, rect.bottom() - 40, 40, 40, 0, -90);
    path.lineTo(rect.bottomLeft() + QPoint(0, 20));
    path.arcTo(rect.left(), rect.bottom() - 40, 40, 40, -90, -90);
    path.lineTo(rect.topLeft());

    painter.fillPath(path, QBrush(QColor(Constants::COLOR_BACKGROUND)));

    // We don't setMask here because we want to compose this with the parent's mask logic
    QWidget::paintEvent(event);
}

void SideWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        // We want to drag the top-level window (MainWindow)
        QWidget *win = window();
        if (win) {
            dragPosition = event->globalPosition().toPoint() - win->frameGeometry().topLeft();
            event->accept();
        }
    }
}

void SideWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        QWidget *win = window();
        if (win) {
            win->move(event->globalPosition().toPoint() - dragPosition);
            event->accept();
        }
    }
}
