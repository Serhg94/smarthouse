#include "buscommand.h"
#include <IOconnector.h>
#include "rc_bus.h"

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
