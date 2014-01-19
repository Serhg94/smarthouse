#include "condition.h"

Condition::Condition(QObject *parent) :
    QObject(parent)
{

}

int Condition::checkCondition(rc_bus *bus){qDebug() << "ERROR";return 200;}
