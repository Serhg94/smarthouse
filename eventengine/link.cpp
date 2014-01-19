#include "link.h"

Link::Link(QObject *parent) :
    QObject(parent)
{
}

void Link::checkLink(rc_bus *bus)
{
    if (!enabled&&do_after&&(event->checkEvent(bus)==1)&&!doing)
    {
        QTimer::singleShot(timeout,this, SLOT(enableLink()));
        doing = true;
    }
    if (enabled&&do_after)
    {
        action->doAction(bus);
        enabled = false;
        doing = false;
    }


    if ((enabled&&(event->checkEvent(bus)==1)&&!do_after))
    {
        action->doAction(bus);
        enabled = false;
        QTimer::singleShot(timeout,this, SLOT(enableLink()));
        return;
    }
    //if (do_after) enabled = false;
}

void Link::setDoAfter()
{
    do_after=true;
    enabled=false;
}

void Link::enableLink()
{
    enabled = true;
}
