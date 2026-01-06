#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QWidget>
#include <QFile>
#include <QFont>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Применение стилей из файла
    QFile file(":/style.css");
    file.open(QFile::ReadOnly);
    a.setStyleSheet(file.readAll());

    // Установка иконки приложения
    a.setWindowIcon(QIcon(":/image/TM.png"));

    // Установка иконки для окна
    MainWindow w;

    // Применение стилей для окна
    w.setStyleSheet("QWidget { border-radius: 50px; }");

    // Убираем рамки окна
    w.setWindowFlags(Qt::FramelessWindowHint);
    w.show();

    return a.exec();
}
