/********************************************************************************
** Form generated from reading UI file 'list.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LIST_H
#define UI_LIST_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_List
{
public:
    QListWidget *listWidget;
    QPushButton *addButton;
    QPushButton *removeButton;

    void setupUi(QWidget *List)
    {
        if (List->objectName().isEmpty())
            List->setObjectName("List");
        List->resize(413, 462);
        listWidget = new QListWidget(List);
        listWidget->setObjectName("listWidget");
        listWidget->setGeometry(QRect(20, 20, 371, 341));
        listWidget->setLayoutDirection(Qt::LeftToRight);
        listWidget->setLayoutMode(QListView::SinglePass);
        listWidget->setItemAlignment(Qt::AlignHCenter);
        addButton = new QPushButton(List);
        addButton->setObjectName("addButton");
        addButton->setGeometry(QRect(20, 380, 181, 61));
        QFont font;
        font.setFamilies({QString::fromUtf8("Century Gothic")});
        font.setPointSize(12);
        font.setBold(false);
        font.setItalic(false);
        addButton->setFont(font);
        addButton->setLayoutDirection(Qt::RightToLeft);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/image/add.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        addButton->setIcon(icon);
        removeButton = new QPushButton(List);
        removeButton->setObjectName("removeButton");
        removeButton->setGeometry(QRect(220, 380, 171, 61));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Century Gothic")});
        font1.setPointSize(12);
        removeButton->setFont(font1);
        removeButton->setLayoutDirection(Qt::RightToLeft);
        removeButton->setLocale(QLocale(QLocale::Ukrainian, QLocale::Ukraine));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/image/remove.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        removeButton->setIcon(icon1);

        retranslateUi(List);

        QMetaObject::connectSlotsByName(List);
    } // setupUi

    void retranslateUi(QWidget *List)
    {
        List->setWindowTitle(QCoreApplication::translate("List", "Form", nullptr));
        addButton->setText(QCoreApplication::translate("List", " Create ", nullptr));
        removeButton->setText(QCoreApplication::translate("List", " Remove ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class List: public Ui_List {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LIST_H
