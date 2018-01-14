#include <QCoreApplication>
#include <stdio.h>
#include <stdlib.h>
#include <QDebug>
#include <QtMsgHandler>
#ifdef Q_OS_WIN
	#include <windows.h>
#endif

//#include "mainwindow.h"
//#include "alarmdialog.h"
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
        printf("%s\n", msg.toLocal8Bit().constData());
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
    QCoreApplication a(argc, argv);
    controller control;
    control.init();
    return a.exec();
}
