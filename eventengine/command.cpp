#include "command.h"
#include <QDebug>

Command::Command(QObject *parent) :
    QObject(parent)
{
}

int Command::doCommand(){qDebug() << "ERROR";return 200;}
