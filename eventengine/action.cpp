#include "action.h"

Action::Action(QObject *parent) :
    QObject(parent)
{
}

int Action::doAction(rc_bus *bus)
{
    for (int i = 0; i < commands.size(); ++i) {
        commands.at(i)->doCommand(bus);
    }
    return 1;
}
