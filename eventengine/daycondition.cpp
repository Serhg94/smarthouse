#include "daycondition.h"

dayCondition::dayCondition(QObject *parent) :
    Condition(parent)
{
}

// 1 - выполняется, 0 - не выполняется, 6 - что то пошло не так.
int dayCondition::checkCondition()
{
    QDate cur = QDate::currentDate();
    switch(type)
    {
    case 1:
        if (cur.dayOfWeek()==day) return 1;
        else return 0;
        break;
    case 2:
        if (cur.dayOfWeek()!=day) return 1;
        else return 0;
        break;
    }
    return 6;
}
