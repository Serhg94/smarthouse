#include "buscommand.h"

busCommand::busCommand(QObject *parent) :
    Command(parent)
{
}

int busCommand::doCommand(rc_bus *bus)
{
    bus->sendStr(comm);
    //qDebug() << "bus command sended:"<<comm;
}
