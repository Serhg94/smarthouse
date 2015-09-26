#include "link.h"
#include <QTime>

#include <QDebug>
#include <QThread>

Link::Link(QObject *parent) :
    QObject(parent)
{
    enabled = true;
    do_after = false;
    once_check = false;
    doing = false;
}

void Link::init()
{
    do_after_timer = new QTimer();
    do_after_timer->setSingleShot(true);
    QObject::connect(do_after_timer, SIGNAL(timeout()), this, SLOT(enableLink()));

    //timer = new QTimer();
    //QObject::connect(timer, SIGNAL(timeout()), this, SLOT(checkLink()));
    //timer->start(10);
}


//экспериментально
void Link::checkStart()
{
    timer = new QTimer();
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(checkLink()));
    timer->start(10);
}
//-----------------

void Link::checkLink()
{
    qDebug() << count;

    try
        {
        //if (!enabled&&do_after&&(event->checkEvent(bus)==1)&&!doing)
        if (!enabled&&do_after&&(event->checkEvent()==1))
        {
            do_after_timer->stop();
            do_after_timer->start(timeout);
            //QTimer::singleShot(timeout,this, SLOT(enableLink()));
            //doing = true;
        }
        if (enabled&&do_after)
        {
            enabled = false;
            action->doAction();
            //doing = false;
        }


        if (!enabled&&once_check&&(event->checkEvent()==1)&&!doing)
        {
            do_after_timer->start(timeout);
            //QTimer::singleShot(timeout,this, SLOT(enableLink()));
            doing = true;
        }
        if (enabled&&once_check)
        {
            enabled = false;
            doing = false;
            action->doAction();
        }


        if ((enabled&&(event->checkEvent()==1)&&!do_after&&!once_check))
        {
            enabled = false;
            do_after_timer->start(timeout);
            action->doAction();
            //QTimer::singleShot(timeout,this, SLOT(enableLink()));
            return;
        }
        //if (do_after) enabled = false;
    }
    catch(...)
    {
        qDebug()<<" Ошибка проверки условий";
    }
    qDebug() << count;
    count++;
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
