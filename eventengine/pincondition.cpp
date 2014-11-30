#include "pincondition.h"

pinCondition::pinCondition(QObject *parent) :
    Condition(parent)
{
    oldState = -1;
}

int pinCondition::pinState()
{
    switch(pintype)
    {
    case 1:
        {if (io_connector->bus->sets[mk][pin]=='1') return 1; else return 0;}
        break;
    case 2:
        {if (io_connector->bus->rebs[mk][pin]=='1') return 1; else return 0;}
        break;
    case 3:
        {if (io_connector->bus->butt[mk][pin]=='1') return 1; else return 0;}
        break;
    }
    return -1;
}


void pinCondition::init()
{
    switch(pintype)
    {
    case 1:
        //if (oldState==-1)
        {if (io_connector->bus->sets[mk][pin]=='1') oldState = 1; else oldState = 0;}
        break;
    case 2:
        //if (oldState==-1)
        {if (io_connector->bus->rebs[mk][pin]=='1') oldState = 1; else oldState = 0;}
        break;
    case 3:
        //if (oldState==-1)
        {if (io_connector->bus->butt[mk][pin]=='1') oldState = 1; else oldState = 0;}
        break;
    }
}

// 1- пин изменился на 1, 2 - пин изменился на 0, 3 - пин равен 1, 4 - пин равен 0.
int pinCondition::checkCondition()
{
    switch(type)
    {
    case 1:
        if ((oldState==0)&&(pinState()==1))
        {
            init();
            return 1;
        }
        break;
    case 2:
        if ((oldState==1)&&(pinState()==0))
        {
            init();
            return 1;
        }
        break;
    case 3:
        if (pinState()==1)
        {
            return 1;
        }
        break;
    case 4:
        if (pinState()==0)
        {
            return 1;
        }
        break;
    }
    init();
    return 0;
}







