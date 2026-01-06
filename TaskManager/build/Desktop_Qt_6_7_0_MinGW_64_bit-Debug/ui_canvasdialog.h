/********************************************************************************
** Form generated from reading UI file 'canvasdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CANVASDIALOG_H
#define UI_CANVASDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CanvasDialog
{
public:

    void setupUi(QWidget *CanvasDialog)
    {
        if (CanvasDialog->objectName().isEmpty())
            CanvasDialog->setObjectName("CanvasDialog");
        CanvasDialog->resize(400, 300);

        retranslateUi(CanvasDialog);

        QMetaObject::connectSlotsByName(CanvasDialog);
    } // setupUi

    void retranslateUi(QWidget *CanvasDialog)
    {
        CanvasDialog->setWindowTitle(QCoreApplication::translate("CanvasDialog", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CanvasDialog: public Ui_CanvasDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CANVASDIALOG_H
