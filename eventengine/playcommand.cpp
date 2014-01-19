#include "playcommand.h"

playCommand::playCommand(QObject *parent) :
    Command(parent)
{
}


int playCommand::doCommand(rc_bus *bus)
{
    pl->add(file);
}
