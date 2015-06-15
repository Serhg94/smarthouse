#include <QApplication>
#include <stdio.h>
#include <stdlib.h>
#include <QDebug>
#include <QtMsgHandler>

#include "mainwindow.h"
#include "alarmdialog.h"
#include "controller.h"

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString s = QTime::currentTime().toString()+" "+QDate::currentDate().toString()+" "+msg;
    QByteArray localMsg = s.toLocal8Bit();
    FILE *file; // указатель на файл, в который пишем
    file = fopen("file.log", "a"); // открываем файл на запись
    switch (type) {
    case QtDebugMsg:
        fprintf(file, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        fprintf(file, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(file, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(file, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        abort();
    }
    fclose(file);
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput);
    QApplication a(argc, argv);
    alarmDialog budil;
    QThread controlThread;
    controller control;
    control.moveToThread(&controlThread);
    QObject::connect(&controlThread, SIGNAL(started()), &control, SLOT(init()));
    controlThread.start();
    MainWindow view(&a);
    QObject::connect(&view, SIGNAL(alarmWindowOpen()), &budil, SLOT(show()));
    QObject::connect(&budil, SIGNAL(Alarm(int,QString)), &control, SLOT(bud_action(int,QString)));
    QObject::connect(&view, SIGNAL(openPort(QString)), &control, SLOT(openPort(QString)));
    QObject::connect(&control, SIGNAL(toLog(QString)), &view, SLOT(appendStr(QString)));
    QObject::connect(&control, SIGNAL(RefreshView(int,QString)), &view, SLOT(RefreshView(int,QString)));
    QObject::connect(&a, SIGNAL(aboutToQuit()), &controlThread, SLOT(quit()));
    view.show();
    return a.exec();
}
