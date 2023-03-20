//#include "mainwindow.h"
#include "splashscreen.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SplashScreen w;
    w.show();
    return a.exec();
}
