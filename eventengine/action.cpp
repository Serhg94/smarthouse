#include "action.h"

Action::Action(QObject *parent) :
    QObject(parent)
{
}

int Action::doAction()
{
    for (int i = 0; i < commands.size(); ++i) {
        commands.at(i)->doCommand();
    }
    return 1;
}
