#include "saycommand.h"

sayCommand::sayCommand(QObject *parent) :
    Command(parent)
{
}

int sayCommand::doCommand()
{
    switch(type)
    {
    case 1:
        speakTime();
        if (io_connector->bus->_debug)
            qDebug() << " Say time";
        break;
    case 2:
        speakTerm();
        if (io_connector->bus->_debug)
            qDebug() << " Say temper = "<< io_connector->termo->temper;
        break;
    }
    return 1;
}

void sayCommand::speakTerm()
{
    int t = io_connector->termo->temper;
    if ((t >40)||(t<-40)) return;
    io_connector->player->add("/temper/temp.wav");
    QString s;  s = QString("/temper/%1.wav")
            .arg(t);
    io_connector->player->add(s);
    //qDebug() << s;
}


void sayCommand::speakTime()
{
    QTime cur = QTime::currentTime();
    io_connector->player->add("vremya.wav");
    QString s;  s = QString("/hours/%1.wav")
            .arg(cur.hour());
    io_connector->player->add(s);
    //qDebug() << s;
    s = QString("/minutes/%1.wav")
                .arg(cur.minute());
    io_connector->player->add(s);
    //qDebug() << s;
}
