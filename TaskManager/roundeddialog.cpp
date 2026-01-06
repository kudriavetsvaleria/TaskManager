#include "roundeddialog.h"

RoundedDialog::RoundedDialog(QWidget *parent, int radius)
    : QDialog(parent), m_radius(radius) {
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    updateMask();
}

RoundedDialog::~RoundedDialog() {
}

void RoundedDialog::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(this->rect(), m_radius, m_radius);
    painter.fillPath(path, QBrush(QColor("#0c0f1b")));
    QDialog::paintEvent(event);
}

void RoundedDialog::updateMask() {
    QRegion region(this->rect(), QRegion::Rectangle);
    QRegion roundedRegion(this->rect().adjusted(1, 1, -1, -1), QRegion::Rectangle);
    QRegion mask = QRegion(0, 0, width(), height(), QRegion::Ellipse);
    setMask(mask);
}
