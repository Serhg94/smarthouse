#include "command.h"

Command::Command(QObject *parent) :
    QObject(parent)
{
}

int Command::doCommand(){qDebug() << "ERROR";return 200;}
