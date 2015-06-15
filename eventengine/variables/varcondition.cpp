#include "varcondition.h"

varCondition::varCondition(QObject *parent) :
    Condition(parent)
{
}

int varCondition::checkCondition()
{
    if (io_connector->vars == NULL) return 0;
    if ((number>=100)||(number<0)) return 0;
    //1-больше, 2-меньше, 3-равно, 4-не равно
    switch(type)
    {
    case 1:
        if (io_connector->vars->at(number)>value) return 1; else return 0;
        break;
    case 2:
        if (io_connector->vars->at(number)<value) return 1; else return 0;
        break;
    case 3:
        if (io_connector->vars->at(number)==value) return 1; else return 0;
        break;
    case 4:
        if (io_connector->vars->at(number)!=value) return 1; else return 0;
        break;
    }
    return 6;
}
