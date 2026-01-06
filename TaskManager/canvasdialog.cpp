#include "canvasdialog.h"
#include "ui_canvasdialog.h"

CanvasDialog::CanvasDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CanvasDialog)
{
    ui->setupUi(this);
}

CanvasDialog::~CanvasDialog()
{
    delete ui;
}
