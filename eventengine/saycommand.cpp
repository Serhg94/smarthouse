#include "saycommand.h"

sayCommand::sayCommand(QObject *parent) :
    Command(parent)
{
}

int sayCommand::doCommand(rc_bus *bus)
{
    switch(type)
    {
    case 1:
        speakTime();
        break;
    case 2:
        speakTerm();
        //qDebug() << "temper = "<< tem->temper;
        break;
    }
}

void sayCommand::speakTerm()
{
    int t = tem->temper;
    if ((t >40)||(t<-40)) return;
    pl->add("/temper/temp.wav");
    QString s;  s = QString("/temper/%1.wav")
            .arg(t);
    pl->add(s);
    //qDebug() << s;
}


void sayCommand::speakTime()
{
    QTime cur = QTime::currentTime();
    pl->add("vremya.wav");
    QString s;  s = QString("/hours/%1.wav")
            .arg(cur.hour());
    pl->add(s);
    //qDebug() << s;
    s = QString("/minutes/%1.wav")
                .arg(cur.minute());
    pl->add(s);
    //qDebug() << s;
}
