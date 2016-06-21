#include "variables.h"

variables::variables(QObject *parent) :
    QObject(parent)
{
    for (int i = 0; i < VAR_COUNT; i++) {
        vars.append(0.0);
        value_generators.append((new MathExpression()));
    }
}

void variables::changeValue(int num, double val)
{
    mutex.lock();
    double old_val = vars[num];
    vars[num] = val;
    mutex.unlock();
    if (old_val != val)
        emit valueChanged(QString("var%1").arg(num));
}

double variables::at(int num)
{
    mutex.lock();
    double val = vars[num];
    mutex.unlock();
    return val;
}

