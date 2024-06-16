
#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QWidget>
#include <QFile>
#include <QFont>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile file(":/style.css");
    file.open(QFile::ReadOnly);
    a.setStyleSheet(file.readAll());

    MainWindow w;
    w.setStyleSheet("QWidget { border-radius: 50px;}");
    w.setWindowFlags(Qt::FramelessWindowHint);
    w.show();


    return a.exec();
}
