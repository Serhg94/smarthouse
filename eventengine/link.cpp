#include "link.h"
#include <QTime>

Link::Link(QObject *parent) :
    QObject(parent)
{
    enabled = true;
    do_after_timer = new QTimer(this);
    do_after_timer->setSingleShot(true);
    QObject::connect(do_after_timer, SIGNAL(timeout()), this, SLOT(enableLink()));
    do_after = false;
    once_check = false;
    doing = false;
}

void Link::checkLink(rc_bus *bus)
{
    try
        {
        //if (!enabled&&do_after&&(event->checkEvent(bus)==1)&&!doing)
        if (!enabled&&do_after&&(event->checkEvent(bus)==1))
        {
            do_after_timer->stop();
            do_after_timer->start(timeout);
            //QTimer::singleShot(timeout,this, SLOT(enableLink()));
            //doing = true;
        }
        if (enabled&&do_after)
        {
            action->doAction(bus);
            enabled = false;
            //doing = false;
        }


        if (!enabled&&once_check&&(event->checkEvent(bus)==1)&&!doing)
        {
            do_after_timer->start(timeout);
            //QTimer::singleShot(timeout,this, SLOT(enableLink()));
            doing = true;
        }
        if (enabled&&once_check)
        {
            action->doAction(bus);
            enabled = false;
            doing = false;
        }


        if ((enabled&&(event->checkEvent(bus)==1)&&!do_after&&!once_check))
        {
            action->doAction(bus);
            enabled = false;
            do_after_timer->start(timeout);
            //QTimer::singleShot(timeout,this, SLOT(enableLink()));
            return;
        }
        //if (do_after) enabled = false;
    }
    catch(...)
    {
        qDebug()<<QTime::currentTime().toString()+ " Ошибка проверки условий";
    }
}

void Link::setDoAfter()
{
    do_after=true;
    enabled=false;
}

void Link::setDoAfterOnceCheck()
{
    once_check=true;
    enabled=false;
}

void Link::enableLink()
{
    enabled = true;
}
