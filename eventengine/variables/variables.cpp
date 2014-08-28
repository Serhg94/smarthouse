#include "variables.h"

variables::variables(QObject *parent) :
    QObject(parent)
{
    for (int i = 0; i < VAR_COUNT; ++i) {
        vars.append(0);
    }
}

void variables::changeValue(int num, int val)
{
    vars[num] = val;
    emit valueChanged(num, val);
}
