#include "condition.h"

Condition::Condition(QObject *parent) :
    QObject(parent)
{

}

int Condition::checkCondition(){qDebug() << "ERROR";return 200;}
