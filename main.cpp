#include "Navigation.h"
#include "mainwindow.h"
#include <QApplication>
#include "head.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
