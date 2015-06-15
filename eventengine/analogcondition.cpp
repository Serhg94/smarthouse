#include "analogcondition.h"

analogCondition::analogCondition(QObject *parent) :
    Condition(parent)
{
}

int analogCondition::checkCondition()
{
    io_connector->bus->read_mutex.lock();
    int val = io_connector->bus->stat[mk][valtype];
    io_connector->bus->read_mutex.unlock();
    switch(type)
    {
    case 1:
        if (val==212) return 6;
        if (val>value) return 1; else return 0;
        break;
    case 2:
        if (val==212) return 6;
        if (val<value) return 1; else return 0;
        break;
    }
    return 6;
}
