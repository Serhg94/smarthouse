#include "linktimer.h"

Linktimer::Linktimer(QObject *parent) :
    QObject(parent)
{
}

void Linktimer::startInOneThread()
{
    timer = new QTimer();
    for (int i = 0; i < links.size(); ++i) {
        links.at(i)->init();
    }
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(checkLinks()));
    timer->start(10);
}

void Linktimer::startInManyThreads()
{
    for (int i = 0; i < links.size(); ++i) {
        link_threads.append(new QThread());
        links.at(i)->moveToThread(link_threads.at(i));
        link_threads.at(i)->start();
        QObject::connect(link_threads.at(i), SIGNAL(started()), links.at(i), SLOT(init()));
        //links.at(i)->checkStart(bus);
    }
}

void Linktimer::checkLinks()
{
    for (int i = 0; i < links.size(); ++i) {
        links.at(i)->checkLink();
    }
}
