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

    // Установка иконки для окна
    MainWindow w;
    w.setWindowIcon(QIcon(":images/TM.png"));  // Устанавливаем иконку из ресурсов

    // Применение стилей для окна
    w.setStyleSheet("QWidget { border-radius: 50px; }");

    // Убираем рамки окна
    w.setWindowFlags(Qt::FramelessWindowHint);
    w.show();

    return a.exec();
}
