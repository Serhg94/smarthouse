#ifndef QMAIN_H
#define QMAIN_H

#include "mainwindow.h"
#include "alarmdialog.h"
#include "controller.h"

class Qmain : public QObject
{
    Q_OBJECT
public:
    alarmDialog *budil;
    controller *control;
    MainWindow *view;
    explicit Qmain(QApplication *a, QObject *parent = NULL)
    {
        budil = new alarmDialog;
        control = new controller;
        control->start();
        view = new MainWindow(a);
        QObject::connect(view, SIGNAL(alarmWindowOpen()), budil, SLOT(show()));
        QObject::connect(budil, SIGNAL(Alarm(int,QString)), control, SLOT(bud_action(int,QString)));
        QObject::connect(view, SIGNAL(openPort(QString)), control, SLOT(openPort(QString)));
        QObject::connect(control, SIGNAL(toLog(QString)), view, SLOT(appendStr(QString)));
        QObject::connect(control, SIGNAL(RefreshView(int,QString)), view, SLOT(RefreshView(int,QString)));
        view->show();
    }

};

#endif // QMAIN_H
