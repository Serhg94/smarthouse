#include "controller.h"
#include <QStringListModel>



controller::controller(QObject *parent) :
    QObject(parent)
{
}

void controller::init()
{
    io_connector = new IOconnector(this);

    QObject::connect(io_connector->udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
    QObject::connect(io_connector->bus, SIGNAL(gettedString(QString)), this, SLOT(appendStr(QString)));
    QObject::connect(io_connector->bus, SIGNAL(sendedString(QString)), this, SLOT(appendStr(QString)));
    QObject::connect(io_connector->bus, SIGNAL(gettedString(QString)), this, SLOT(sendDatagram(QString)));
    QObject::connect(io_connector->bus, SIGNAL(statsChanged(int)), this, SLOT(sendToView(int)));
    QObject::connect(io_connector->vars, SIGNAL(valueChanged(int,int)), this, SLOT(sendVariables()));
    // подключение CELAC
    linkengine = makeLinksFromFile("scripts.txt", io_connector);
    many_thread = 0;
    //if (linkengine->links.size()<30)
    //{
        //linkengine->moveToThread(&link_thread);
        //link_thread.start();
        //QObject::connect(&link_thread, SIGNAL(started()), linkengine, SLOT(startInOneThread()));
        //linkengine->startInOneThread();
    //}
    //else
        //linkengine->startInManyThreads();

    readConfig("config.ini");
    QObject::connect(this, SIGNAL(toLog(QString)), this, SLOT(_debugInfo(QString)));

    if (this->many_thread==1)
    {
        linkengine->startInManyThreads();
        qDebug() << " Обработка линков в разных потоках";
    }
    else
    {
        //linkengine->moveToThread(&link_thread);
        //link_thread.start();
        //QObject::connect(&link_thread, SIGNAL(started()), linkengine, SLOT(startInOneThread()));
        linkengine->startInOneThread();
        qDebug() << " Обработка линков в одном потоке";
    }

    up_timer = new QTimer();
    QObject::connect(up_timer, SIGNAL(timeout()), this, SLOT(update()));
    up_timer->start(UPT_MSEC);

    main_control_timer = new QTimer();
    QObject::connect(main_control_timer, SIGNAL(timeout()), this, SLOT(main_control()));
    main_control_timer->start(MCT_MSEC);

    maintain_timer = new QTimer();
    QObject::connect(maintain_timer, SIGNAL(timeout()), this, SLOT(maintain()));
    maintain_timer->start(MAINTAIN_MSEC);

}


void controller::readConfig(QString name)
{
    try
    {
        QFile file(name);
        if (!file.open(QIODevice::ReadOnly)) { // Проверяем, возможно ли открыть наш файл для чтения
            qDebug()<<" Не могу найти файл настроек";
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
                    else if (list[0].contains("var")){
                        int num = list[0].mid(str.indexOf("[")+1,
                                              str.indexOf("]")-str.indexOf("[")-1).toInt();
                        io_connector->vars->changeValue(num, list[1].toInt());
                    }
                }

            }
            lineLength = file.readLine(buf, sizeof(buf));
        }
        qDebug()<<" Настройки загружены";
        return;
    }
    catch(...)
    {
        qDebug()<<" Ошибка разбора файла настроек";
        //QMessageBox::critical(NULL,QObject::tr("Ошибка"),QObject::tr("Ошибка разбора файла скриптов!"));
    }
}

void controller::_debugInfo(QString msg)
{
    if (io_connector->bus->_debug)
        qDebug() << " " << msg;
}

void controller::openPort(QString s)
{
    io_connector->bus->open_port(NULL, s);
}


void controller::appendStr(QString info)
{
    emit toLog(info);
}


void controller::update()
{
    //for(int i = 0; i<=10; i++)
    //{
    //    io_connector->bus->stat[i][1]=0;
    //}
    emit RefreshView(1, "1 - OFFLINE");
    emit RefreshView(2, "2 - OFFLINE");
    emit RefreshView(3, "3 - OFFLINE");
    emit RefreshView(4, "4 - OFFLINE");
    emit RefreshView(5, "5 - OFFLINE");
    emit RefreshView(6, "6 - OFFLINE");

    io_connector->bus->sendStr("clr");
    //QString a("clr");
    //a[a.length()]='\n';
    //io_connector->bus->serial->write(a.toLatin1());
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


void controller::bud_action(int num, QString action)
{
    io_connector->player->add("bud.wav");
    speakTime();
    io_connector->bus->sendStr(action);
}


void controller::main_control()
{

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
    emit RefreshView(sn, s);
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
        if (data[data.length()-1]!='\n') data[data.length()] = '\n';
                emit toLog(data);
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
        if (data.contains("bud"))
        {

        }
        else
        if (data.contains("getio_connector->vars"))
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
        // посылаем состояния всех галок, с большой буквы - чтоб не путалось с запросами от других клиентов
//        if (data.contains("getoptions"))
//        {
//            QString s;  s = QString("Getoptions%1%2%3%4%5")
//                    .arg((int)ui->sayinfoBox->isChecked())
//                    .arg((int)ui->ddnigthBox->isChecked())
//                    .arg((int)ui->dooralertBox->isChecked())
//                    .arg((int)ui->lockalertBox->isChecked())
//                    .arg((int)ui->doorlightBox->isChecked());
//            sendDatagram(s);
//        }
//        else
//        // установка присланных состояний галок
//        if (data.contains("options"))
//        {
//            if (data[7]=='1') ui->sayinfoBox->setChecked(true);
//            else if (data[7]=='0') ui->sayinfoBox->setChecked(false);
//            if (data[8]=='1') ui->ddnigthBox->setChecked(true);
//            else if (data[8]=='0') ui->ddnigthBox->setChecked(false);
//            if (data[9]=='1') ui->dooralertBox->setChecked(true);
//            else if (data[9]=='0') ui->dooralertBox->setChecked(false);
//            if (data[10]=='1') ui->lockalertBox->setChecked(true);
//            else if (data[10]=='0') ui->lockalertBox->setChecked(false);
//            if (data[11]=='1') ui->doorlightBox->setChecked(true);
//            else if (data[11]=='0') ui->doorlightBox->setChecked(false);
//        }
//        else
        //qDebug()<<datagram.data();
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
    io_connector->~IOconnector();
}

