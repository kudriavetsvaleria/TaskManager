#ifndef ROUNDEDDIALOG_H
#define ROUNDEDDIALOG_H

#include <QDialog>
#include <QPainter>
#include <QPainterPath>

class RoundedDialog : public QDialog {
    Q_OBJECT

public:
    explicit RoundedDialog(QWidget *parent = nullptr, int radius = 20);
    ~RoundedDialog();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int m_radius;
    void updateMask();
};

#endif // ROUNDEDDIALOG_H
