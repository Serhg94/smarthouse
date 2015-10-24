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
    event_gen_onstart = false;
    event_gen_onend = false;
    idlink = -1;
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

void Link::sendEvent()
{
    QString s;  s = QString("INSERT INTO `smarthouse`.`events` (`time`, `description`, `link_idlink`) VALUES ('%1', '%2', '%3')")
            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(description).arg(idlink);
    io_connector->sql_db->addNonAnswerRequest(s);
}

void Link::checkLink()
{
    try
        {
        //if (!enabled&&do_after&&(event->checkEvent(bus)==1)&&!doing)
        if (!enabled&&do_after&&(event->checkEvent()==1))
        {
            do_after_timer->stop();
            do_after_timer->start(timeout);
            if (event_gen_onstart)
                sendEvent();
            //QTimer::singleShot(timeout,this, SLOT(enableLink()));
            //doing = true;
        }
        if (enabled&&do_after)
        {
            enabled = false;
            action->doAction();
            if (event_gen_onend)
                sendEvent();
            //doing = false;
        }


        if (!enabled&&once_check&&(event->checkEvent()==1)&&!doing)
        {
            do_after_timer->start(timeout);
            if (event_gen_onstart)
                sendEvent();
            //QTimer::singleShot(timeout,this, SLOT(enableLink()));
            doing = true;
        }
        if (enabled&&once_check)
        {
            enabled = false;
            doing = false;
            action->doAction();
            if (event_gen_onend)
                sendEvent();
        }


        if ((enabled&&(event->checkEvent()==1)&&!do_after&&!once_check))
        {
            enabled = false;
            do_after_timer->start(timeout);
            action->doAction();
            if (event_gen_onend)
                sendEvent();
            //QTimer::singleShot(timeout,this, SLOT(enableLink()));
            return;
        }
        //if (do_after) enabled = false;
    }
    catch(...)
    {
        qDebug()<<" Ошибка проверки условий";
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
