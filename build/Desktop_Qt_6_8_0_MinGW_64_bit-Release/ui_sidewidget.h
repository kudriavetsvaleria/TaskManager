/********************************************************************************
** Form generated from reading UI file 'sidewidget.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIDEWIDGET_H
#define UI_SIDEWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SideWidget
{
public:
    QPushButton *open;

    void setupUi(QWidget *SideWidget)
    {
        if (SideWidget->objectName().isEmpty())
            SideWidget->setObjectName("SideWidget");
        SideWidget->resize(413, 66);
        open = new QPushButton(SideWidget);
        open->setObjectName("open");
        open->setGeometry(QRect(0, -20, 421, 101));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/image/arrow.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        open->setIcon(icon);
        open->setIconSize(QSize(60, 60));

        retranslateUi(SideWidget);

        QMetaObject::connectSlotsByName(SideWidget);
    } // setupUi

    void retranslateUi(QWidget *SideWidget)
    {
        open->setText(QString());
        (void)SideWidget;
    } // retranslateUi

};

namespace Ui {
    class SideWidget: public Ui_SideWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIDEWIDGET_H
