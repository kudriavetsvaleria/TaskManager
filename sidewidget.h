#ifndef SIDEWIDGET_H
#define SIDEWIDGET_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
class SideWidget;
}

class SideWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SideWidget(QWidget *parent = nullptr);
    ~SideWidget();
    QPushButton* getOpenButton();

protected:
    void paintEvent(QPaintEvent *event) override;
    
    // Allow dragging from here to move the parent window
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    Ui::SideWidget *ui;
    QPoint dragPosition;
};

#endif // SIDEWIDGET_H
