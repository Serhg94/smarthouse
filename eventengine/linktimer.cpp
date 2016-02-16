#include "linktimer.h"

Linktimer::Linktimer(IOconnector * io, QObject *parent) :
    QObject(parent)
{
    this->io_connector = io;
}

void Linktimer::startInOneThread()
{
    timer = new QTimer();
    for (int i = 0; i < links.size(); ++i) {
        links.at(i)->init();
    }
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(checkLinks()));
    QObject::connect(this->io_connector->bus, SIGNAL(statsChangedCheck(int)), this, SLOT(checkLinks()));
    QObject::connect(this->io_connector->vars, SIGNAL(valueChanged(int,double)), this, SLOT(checkLinks()));
    timer->start(100);
}

void Linktimer::startInManyThreads()
{
    for (int i = 0; i < links.size(); ++i) {
        link_threads.append(new QThread());
        QObject::connect(link_threads.at(i), SIGNAL(started()), links.at(i), SLOT(init()));
        links.at(i)->moveToThread(link_threads.at(i));
        link_threads.at(i)->start();
        links.at(i)->checkStart();
    }
}

void Linktimer::checkLinks()
{
    //mutex.lock();
    for (int i = 0; i < links.size(); ++i) {
        links.at(i)->checkLink();
    }
    //mutex.unlock();
    //this->io_connector->bus->read_mutex.unlock();
    //this->io_connector->vars->mutex.unlock();
}
