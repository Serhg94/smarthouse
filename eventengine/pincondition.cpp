#include "pincondition.h"

pinCondition::pinCondition(QObject *parent) :
    Condition(parent)
{
}

int pinCondition::pinState(rc_bus *bus)
{
    switch(pintype)
    {
    case 1:
        {if (bus->sets[mk][pin]=='1') return 1; else return 0;}
        break;
    case 2:
        {if (bus->rebs[mk][pin]=='1') return 1; else return 0;}
        break;
    case 3:
        {if (bus->butt[mk][pin]=='1') return 1; else return 0;}
        break;
    }
}


void pinCondition::init(rc_bus *bus)
{
    switch(pintype)
    {
    case 1:
        //if (oldState==-1)
        {if (bus->sets[mk][pin]=='1') oldState = 1; else oldState = 0;}
        break;
    case 2:
        //if (oldState==-1)
        {if (bus->rebs[mk][pin]=='1') oldState = 1; else oldState = 0;}
        break;
    case 3:
        //if (oldState==-1)
        {if (bus->butt[mk][pin]=='1') oldState = 1; else oldState = 0;}
        break;
    }
}

// 1- пин изменился на 1, 2 - пин изменился на 0, 3 - пин равен 1, 4 - пин равен 0.
int pinCondition::checkCondition(rc_bus *bus)
{
    switch(type)
    {
    case 1:
        if ((oldState==0)&&(pinState(bus)==1))
        {
            init(bus);
            return 1;
        }
        break;
    case 2:
        if ((oldState==1)&&(pinState(bus)==0))
        {
            init(bus);
            return 1;
        }
        break;
    case 3:
        if (pinState(bus)==1)
        {
            return 1;
        }
        break;
    case 4:
        if (pinState(bus)==0)
        {
            return 1;
        }
        break;
    }
    init(bus);
    return 0;
}







