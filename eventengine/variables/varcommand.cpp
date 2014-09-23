#include "varcommand.h"

varCommand::varCommand(QObject *parent) :
    Command(parent)
{
}

int varCommand::doCommand()
{
    switch(type)
    {
    case 1:
        io_connector->vars->changeValue(number, value);
        break;
    case 2:
        io_connector->vars->changeValue(number, io_connector->vars->vars.at(number)+value);
        break;
    case 3:
        io_connector->vars->changeValue(number, io_connector->vars->vars.at(number)-value);
        break;
    }
    if (io_connector->bus->_debug)
        qDebug() << " Variable number " << number << " = " << io_connector->vars->vars.at(number);
    return 1;
}
