#include "linktimer.h"

Linktimer::Linktimer(rc_bus *b, QObject *parent) :
    QObject(parent)
{
     bus = b;
}

void Linktimer::start()
{
    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(checkLinks()));
    timer->start(10);
}

void Linktimer::checkLinks()
{
    for (int i = 0; i < links.size(); ++i) {
        links.at(i)->checkLink(bus);
    }
}
