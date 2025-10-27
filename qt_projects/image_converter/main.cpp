#include "mainwindow.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[]) {

    std::cout << "before making Qapp" << std::endl;
    QApplication app(argc, argv);

    std::cout << "before mainwindow object creating" << std::endl;
    MainWindow w;

    std::cout << "before showing window" << std::endl;
    w.show();
    return app.exec();
}