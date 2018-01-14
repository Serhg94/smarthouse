#include "pincondition.h"
#include <IOconnector.h>
#include "rc_bus.h"

pinCondition::pinCondition(QObject *parent) :
    Condition(parent)
{
    oldState = -1;
}

int pinCondition::pinState()
{
    io_connector->bus->read_mutex.lock();
    QChar s = io_connector->bus->sets[mk][pin];
    QChar r = io_connector->bus->rebs[mk][pin];
    QChar b = io_connector->bus->butt[mk][pin];
    io_connector->bus->read_mutex.unlock();
    switch(pintype)
    {
    case 1:
        {if (s=='1') return 1; else if (s=='0') return 0;}
        break;
    case 2:
        {if (r=='1') return 1; else if (r=='0') return 0;}
        break;
    case 3:
        {if (b=='1') return 1; else if (b=='0') return 0;}
        break;
    }
    return -1;
}


void pinCondition::init()
{
    io_connector->bus->read_mutex.lock();
    QChar s = io_connector->bus->sets[mk][pin];
    QChar r = io_connector->bus->rebs[mk][pin];
    QChar b = io_connector->bus->butt[mk][pin];
    io_connector->bus->read_mutex.unlock();
    switch(pintype)
    {
    case 1:
        //if (oldState==-1)
        {if (s=='1') oldState = 1; else if (s=='0') oldState = 0;}
        break;
    case 2:
        //if (oldState==-1)
        {if (r=='1') oldState = 1; else if (r=='0') oldState = 0;}
        break;
    case 3:
        //if (oldState==-1)
        {if (b=='1') oldState = 1; else if (b=='0') oldState = 0;}
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







