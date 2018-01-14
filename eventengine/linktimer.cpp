#include "linktimer.h"
#include "IOconnector.h"
#include "rc_bus.h"

Linktimer::Linktimer(IOconnector * io, QObject *parent) :
    QObject(parent)
{
    this->io_connector = io;
}

void Linktimer::startInOneThread()
{
    timer = new QTimer(this);
    for (int i = 0; i < links.size(); ++i) {
        links.at(i)->init();
    }
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(checkLinks()));
    QObject::connect(this->io_connector->bus, SIGNAL(statsChangedCheck(QString)), this, SLOT(checkDepLinks(QString)));
    QObject::connect(this->io_connector->vars, SIGNAL(valueChanged(QString)), this, SLOT(checkDepLinks(QString)));
    timer->start(1000);
}

void Linktimer::startInManyThreads()
{
    for (int i = 0; i < links.size(); ++i) {
        link_threads.append(new QThread(this));
        QObject::connect(link_threads.at(i), SIGNAL(started()), links.at(i), SLOT(init_multy_thread()));
        links.at(i)->moveToThread(link_threads.at(i));
        link_threads.at(i)->start();
        QObject::connect(this->io_connector->bus, SIGNAL(statsChangedCheck(QString)), links.at(i), SLOT(checkDepLink(QString)));
        QObject::connect(this->io_connector->vars, SIGNAL(valueChanged(QString)), links.at(i), SLOT(checkDepLink(QString)));
    }
}

void Linktimer::checkLinks()
{
    for (int i = 0; i < links.size(); ++i) {
        links.at(i)->checkLink();
    }
}

void Linktimer::checkDepLinks(QString depend)
{
    for (int i = 0; i < links.size(); ++i) {
        if (links.at(i)->dependences.contains(depend, Qt::CaseInsensitive))
            links.at(i)->checkLink();
    }
}

