#include "controller.h"
//#include <QStringListModel>
#include "linkmaker.h"
#include "linkmakerdb.h"
#include "config.h"
#include "sql_worker.h"


controller::controller(QObject *parent) :
    QObject(parent)
{
}

void controller::init()
{
    io_connector = new IOconnector(this);

    QObject::connect(io_connector->udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
    QObject::connect(io_connector->bus, SIGNAL(sendedString(QString)), this, SLOT(appendStr(QString)));
    QObject::connect(io_connector->bus, SIGNAL(gettedString(QString)), this, SLOT(sendDatagram(QString)));
    QObject::connect(io_connector->bus, SIGNAL(statsChanged(int)), this, SLOT(sendToView(int)));
    QObject::connect(io_connector->vars, SIGNAL(valueChanged(QString)), this, SLOT(sendVariables()));
    // подключение CELAC
    linkengine = new Linktimer(io_connector, this);
    many_thread = 0;

    bool db = false;
    bool use_com = false;
    QString com_name = "";
    QString controller_ip = "";
    readConfig("config.ini", db, use_com, com_name, controller_ip);
    io_connector->bus_init(!use_com, com_name, controller_ip);
    io_connector->sql_db->initDB();
    if (db)
        makeLinksFromDB(io_connector, linkengine);
    QObject::connect(this, SIGNAL(toLog(QString)), this, SLOT(_debugInfo(QString)));

    if (this->many_thread==1)
    {
        linkengine->startInManyThreads();
        qDebug() << " Link engine started in multi thread mode";
    }
    else
    {
        linkengine->startInOneThread();
        qDebug() << " Link engine started in single thread mode";
    }

    up_timer = new QTimer(this);
    QObject::connect(up_timer, SIGNAL(timeout()), this, SLOT(update()));
    up_timer->start(UPT_MSEC);

    maintain_timer = new QTimer(this);
    QObject::connect(maintain_timer, SIGNAL(timeout()), this, SLOT(maintain()));
    maintain_timer->start(MAINTAIN_MSEC);

}


void controller::readConfig(QString name, bool &links_from_db, bool &use_com, QString &com_name, QString &server_ip)
{
    try
    {
        QFile file(name);
        if (!file.open(QIODevice::ReadOnly)) { // Проверяем, возможно ли открыть наш файл для чтения
            qDebug()<<" Cant open config.ini";
            return; // если это сделать невозможно, то завершаем функцию
        }
        char buf[2048];
        qint64 lineLength = file.readLine(buf, sizeof(buf));
        while(lineLength != -1)
        {
            QString str(buf);
            if (!str.contains("//"))
            {
                QStringList list = str.split("=");
                if (list.size()==2)
                {
                    if (list[0].contains("debug")){
                        if (list[1].contains("1")){
                            io_connector->bus->_debug = true;
                            io_connector->termo->_debug = true;
                        }
                    }
                    if (list[0].contains("multithreading")){
                        if (list[1].contains("1")){
                            this->many_thread = 1;
                        }
                    }
                    if (list[0].contains("links_from_db")){
                        if (list[1].contains("1")){
                            links_from_db = true;
                        }
                    }
                    if (list[0].contains("use_com")){
                        if (list[1].contains("1")){
                            use_com = true;
                        }
                    }
                    if (list[0].contains("com_port")){
                        com_name = list[1].simplified();
                    }
                    if (list[0].contains("controller_ip")){
                        server_ip = list[1].simplified();
                    }
                    if (list[0].contains("db_addr")){
                        io_connector->sql_db->addr = list[1].simplified();
                    }
                    if (list[0].contains("db_type")){
                        io_connector->sql_db->db_type = list[1].simplified();
                    }
                    if (list[0].contains("db_login")){
                        io_connector->sql_db->login = list[1].simplified();
                    }
                    if (list[0].contains("db_password")){
                        io_connector->sql_db->pass = list[1].simplified();
                    }
                    if (list[0].contains("db_port")){
                        io_connector->sql_db->port = list[1].toInt();
                    }
                    if (list[0].contains("com_wait_answer_timeout")){
                        io_connector->bus->com_wait_answer_timeout = list[1].toInt();
                    }
                    if (list[0].contains("com_poll_delay")){
                        io_connector->bus->com_poll_delay = list[1].toInt();
                    }
                    if (list[0].contains("db_name")){
                        io_connector->sql_db->db_name = list[1].simplified();
                    }
                    if (list[0].contains("links_from_file")){
                        makeLinksFromFile(list[1].simplified(), io_connector, linkengine);
                    }
                    if (list[0].contains("sityID")){
                        io_connector->termo->sity = list[1].simplified();
                    }
                    if (list[0].contains("APIkey")){
                        io_connector->termo->api_key = list[1].simplified();
                    }
                    if (list[0].contains("var")){
                        int num = list[0].mid(str.indexOf("[")+1,
                                              str.indexOf("]")-str.indexOf("[")-1).toInt();
                        bool err = false;
                        if (io_connector->vars->value_generators[num]->Parse(list[1].simplified()))
                        {
                            if (io_connector->vars->value_generators[num]->isConst())
                                io_connector->vars->changeValue(num, io_connector->vars->value_generators[num]->Calculate(err, io_connector));
                            if (err)
                                qDebug() << "Math expression exec error: " << list[1].simplified();
                        }
                        else
                            qDebug() << "Math expression parse error: " << list[1].simplified();
                        //io_connector->vars->changeValue(num, list[1].toDouble());
                    }
                }

            }
            lineLength = file.readLine(buf, sizeof(buf));
        }
        qDebug()<<" config loaded";
        return;
    }
    catch(...)
    {
        qDebug()<<" Config file parse error";
        //QMessageBox::critical(NULL,QObject::tr("Ошибка"),QObject::tr("Ошибка разбора файла скриптов!"));
    }
}

void controller::_debugInfo(QString msg)
{
    if (io_connector->bus->_debug)
        qDebug() << " " << msg;
    else
        printf("%s\n", msg.toLocal8Bit().constData());
}

void controller::openPort(QString s)
{
    io_connector->bus->open_port(QString(), s);
}


void controller::appendStr(QString info)
{
    emit toLog(info);
}


void controller::update()
{
    for (int i = 0; i < VAR_COUNT; i++)
    {
        if (io_connector->vars->value_generators.at(i)->isConst())
            continue;
        bool err = false;
        double val = io_connector->vars->value_generators.at(i)->Calculate(err, io_connector);
        if (err == false)
            io_connector->vars->changeValue(i, val);
        else
            qDebug() << "Math expression exec error: " << io_connector->vars->value_generators.at(i)->primal_str;
    }
    io_connector->bus->initCheck();
}


void controller::speakTerm(int t)
{
    if ((t >40)||(t<-40)) return;
    io_connector->player->add("/temper/temp.wav");
    QString s;  s = QString("/temper/%1.wav")
            .arg(t);
    io_connector->player->add(s);
    //qDebug() << s;
}


void controller::speakTime()
{
    QTime cur = QTime::currentTime();
    QString s;  s = QString("/hours/%1.wav")
            .arg(cur.hour());
    io_connector->player->add(s);
    //qDebug() << s;
    s = QString("/minutes/%1.wav")
                .arg(cur.minute());
    io_connector->player->add(s);
    //qDebug() << s;
}


void controller::maintain()
{
    QString s;  s = QString("temp%1;")
            .arg(io_connector->termo->get_T());
    sendDatagram(s);

}


void controller::sendToView(int sn)
{
    io_connector->bus->read_mutex.lock();
    QString s;  s = QString("S/N:0%1; SETS:%2; GAS:%3; REB:%4; TEM:%5; HUM:%6; BUT:%7;")
            .arg(sn)
            .arg(io_connector->bus->sets[sn])
            .arg(io_connector->bus->stat[sn][0])
            .arg(io_connector->bus->rebs[sn])
            .arg(io_connector->bus->stat[sn][1])
            .arg(io_connector->bus->stat[sn][2])
            .arg(io_connector->bus->butt[sn]);
    io_connector->bus->read_mutex.unlock();
    emit toLog(s);
}


void controller::sendVariables()
{
    QString msg = "vars";
    for (int i = 0; i < VAR_COUNT; ++i) {
        msg += QString("%1;")
                .arg(io_connector->vars->at(i));
    }
    emit toLog(msg);
    sendDatagram(msg);
}


void controller::processPendingDatagrams()
{
    while (io_connector->udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(io_connector->udpSocket->pendingDatagramSize());
        io_connector->udpSocket->readDatagram(datagram.data(), datagram.size());
        QString data = QString(datagram);
        if (data.length()<3) return;
        if (data[data.length()-1]!='\n') data[data.length()] = '\n';
        emit toLog("NET RECEIVED: "+data);
        if (data.contains("temp"))
        {
            speakTerm(io_connector->termo->get_T());
        }
        else
        if (data.contains("time"))
        {
            io_connector->player->add("vremya.wav");
            speakTime();
        }
        else
        if (data.contains("getvars"))
        {
            sendVariables();
        }
        else
        if (data.contains("setvar|"))
        {
            QStringList list = data.split("|");
            if (list.size()==3)
                io_connector->vars->changeValue(list[1].toInt(), list[2].toInt());
        }
        else
            io_connector->bus->sendStr(data);
            //io_connector->bus->serial->write(data.toLatin1());
    }
}


void controller::sendDatagram(QString str)
{
    QByteArray datagram = str.toLatin1();
    io_connector->udpSocket->writeDatagram(datagram.data(), datagram.size(),
                                QHostAddress("127.0.0.1"), PORT_SEND);
    //io_connector->udpSocket->writeDatagram(datagram.data(), datagram.size(),
   //                         QHostAddress::Broadcast, PORT_SEND);
}


controller::~controller()
{
    link_thread.quit();
    link_thread.wait();
    io_connector->deleteLater();
}

