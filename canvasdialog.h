#ifndef CANVASDIALOG_H
#define CANVASDIALOG_H

#include <QWidget>

namespace Ui {
class CanvasDialog;
}

class CanvasDialog : public QWidget
{
    Q_OBJECT

public:
    explicit CanvasDialog(QWidget *parent = nullptr);
    ~CanvasDialog();

private:
    Ui::CanvasDialog *ui;
};

#endif // CANVASDIALOG_H
