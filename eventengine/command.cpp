#include "command.h"

Command::Command(QObject *parent) :
    QObject(parent)
{
}

int Command::doCommand(rc_bus *bus){qDebug() << "ERROR";return 200;}
