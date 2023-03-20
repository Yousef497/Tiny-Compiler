#pragma once

#include <QMainWindow>
#include <ui_splash_screen.h>
#include "circularprogress.h"
#include <mainwindow.h>

class SplashScreen : public QMainWindow
{
    Q_OBJECT

public:
    explicit SplashScreen(QWidget* parent = nullptr);
    ~SplashScreen();

public slots:
    void update();

private:
    int counter = 0;
    Ui_SplashScreen* ui;
    CircularProgress* mprogress;
    QTimer* mtimer;
};


