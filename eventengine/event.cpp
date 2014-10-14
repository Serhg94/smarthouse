#include "eventengine/event.h"

Event::Event(QObject *parent) :
    QObject(parent)
{

}

int Event::checkEvent()
{
    for (int i = 0; i < conditions.size(); i++) {
        if (conditions.at(i)->checkCondition() != 1)
            return 0;
    }
    return 1;
}
