#include "analogcondition.h"

analogCondition::analogCondition(QObject *parent) :
    Condition(parent)
{
}

int analogCondition::checkCondition(rc_bus *bus)
{
    switch(type)
    {
    case 1:
        if (bus->stat[mk][valtype]==212) return 6;
        if (bus->stat[mk][valtype]>value) return 1; else return 0;
        break;
    case 2:
        if (bus->stat[mk][valtype]==212) return 6;
        if (bus->stat[mk][valtype]<value) return 1; else return 0;
        break;
    }
    return 6;
}
