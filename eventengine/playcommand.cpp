#include "playcommand.h"
#include <IOconnector.h>
#include "rc_bus.h"

playCommand::playCommand(QObject *parent) :
    Command(parent)
{
}


int playCommand::doCommand()
{
    if (io_connector->bus->_debug)
        qDebug() << " Play " << file;
    io_connector->player->add(file);
    return 1;
}
