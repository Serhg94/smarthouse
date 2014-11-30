#include "buscommand.h"

busCommand::busCommand(QObject *parent) :
    Command(parent)
{
}

int busCommand::doCommand()
{
    io_connector->bus->sendStr(comm);
    //if (bus->_debug)
        //qDebug() << " Bus send: " << comm;
    return 1;
}
