#include "timecondition.h"

timeCondition::timeCondition(QObject *parent) :
    Condition(parent)
{
}

// 1 - выполняется, 0 - не выполняется, 6 - что то пошло не так.
int timeCondition::checkCondition(rc_bus *bus)
{
    QTime cur = QTime::currentTime();
    switch(type)
    {
    case 1:
        if (cur>time) return 1;
        else return 0;
        break;
    case 2:
        if (cur<time) return 1;
        else return 0;
        break;
    case 3:
        if ((cur.hour()==time.hour())&&
            (cur.minute()==time.minute()))
            return 1;
        else return 0;
        break;
    }
    return 6;
}
