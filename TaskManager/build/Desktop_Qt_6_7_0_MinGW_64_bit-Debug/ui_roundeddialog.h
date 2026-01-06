/********************************************************************************
** Form generated from reading UI file 'roundeddialog.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ROUNDEDDIALOG_H
#define UI_ROUNDEDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_roundeddialog
{
public:

    void setupUi(QWidget *roundeddialog)
    {
        if (roundeddialog->objectName().isEmpty())
            roundeddialog->setObjectName("roundeddialog");
        roundeddialog->resize(400, 300);

        retranslateUi(roundeddialog);

        QMetaObject::connectSlotsByName(roundeddialog);
    } // setupUi

    void retranslateUi(QWidget *roundeddialog)
    {
        roundeddialog->setWindowTitle(QCoreApplication::translate("roundeddialog", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class roundeddialog: public Ui_roundeddialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ROUNDEDDIALOG_H
