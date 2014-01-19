#include "eventengine/event.h"

Event::Event(QObject *parent) :
    QObject(parent)
{

}

int Event::checkEvent(rc_bus *bus)
{
    for (int i = 0; i < conditions.size(); ++i) {
        if (conditions.at(i)->checkCondition(bus) != 1)
            return 0;
    }
    return 1;
}
