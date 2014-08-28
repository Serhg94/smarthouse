#include "varcommand.h"

varCommand::varCommand(QObject *parent) :
    Command(parent)
{
}

int varCommand::doCommand(rc_bus *bus)
{
    vars->changeValue(number, value);
    switch(type)
    {
    case 1:
        vars->changeValue(number, value);
        break;
    case 2:
        vars->changeValue(number, vars->vars.at(number)+value);
        break;
    case 3:
        vars->changeValue(number, vars->vars.at(number)-value);
        break;
    }
    return 1;
}
