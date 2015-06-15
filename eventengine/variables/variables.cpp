#include "variables.h"

variables::variables(QObject *parent) :
    QObject(parent)
{
    for (int i = 0; i < VAR_COUNT; i++) {
        vars.append(0);
    }
}

void variables::changeValue(int num, int val)
{
    mutex.lock();
    vars[num] = val;
    mutex.unlock();
    emit valueChanged(num, val);
}

int variables::at(int num)
{
    mutex.lock();
    int val = vars[num];
    mutex.unlock();
    return val;
}
