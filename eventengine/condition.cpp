#include "condition.h"
#include <QDebug>

Condition::Condition(QObject *parent) :
    QObject(parent)
{

}

int Condition::checkCondition(){qDebug() << "ERROR";return 200;}

QString Condition::getDependences(){qDebug() << "ERROR";return "";}
