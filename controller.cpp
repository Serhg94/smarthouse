#include "controller.h"
#include <QStringListModel>



#define READ_SYCLES 100000
#define MCT_MSEC 300
#define MAINTAIN_MSEC 30000
#define UPT_MSEC 5000
#define PORT_SEND 6668
#define PORT_LISTEN 6669
#define DOOR_ALARM_SEC 300
#define ZAMOK_ALARM_SEC 300
#define SAY_INFO_SEC 1800
#define DOOR_LIGHT_SEC 45


controller::controller(QObject *parent) :
    QObject(parent)
{
}

void controller::init()
{
    //6668 - в него сервер отправляет
    //6669 - его сервер слушает
    udpSocket = new QUdpSocket();
    udpSocket->bind(PORT_LISTEN, QUdpSocket::ShareAddress);

    termo = new web_termometr();
    termo->moveToThread(&tem_thread);
    tem_thread.start();
    QObject::connect(&tem_thread, SIGNAL(started()), termo, SLOT(init()));

    player = new audiosteck();
    player->moveToThread(&audio_thread);
    audio_thread.start();
    QObject::connect(&audio_thread, SIGNAL(started()), player, SLOT(init()));

    bus = new rc_bus(true);
    bus->moveToThread(&bus_thread);
    bus_thread.start();
    QObject::connect(&bus_thread, SIGNAL(started()), bus, SLOT(init()));

    QObject::connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
    QObject::connect(bus, SIGNAL(gettedString(QString)), this, SLOT(appendStr(QString)));
    QObject::connect(bus, SIGNAL(sendedString(QString)), this, SLOT(appendStr(QString)));
    QObject::connect(bus, SIGNAL(gettedString(QString)), this, SLOT(sendDatagram(QString)));
    QObject::connect(bus, SIGNAL(statsChanged(int)), this, SLOT(sendToView(int)));

    vars = new variables();

    // подключение CELAC
    linkengine = makeLinksFromFile("scripts.txt", bus, player, termo, vars);
    if (linkengine->links.size()<30)
    {
        //linkengine->moveToThread(&link_thread);
        //link_thread.start();
        //QObject::connect(&link_thread, SIGNAL(started()), linkengine, SLOT(startInOneThread()));
        linkengine->startInOneThread();
    }
    else
        linkengine->startInManyThreads();

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


void controller::openPort(QString s)
{
    bus->open_port(NULL, s);
}


void controller::appendStr(QString info)
{
    emit toLog(info);
}


void controller::update()
{
    //for(int i = 0; i<=10; i++)
    //{
    //    bus->stat[i][1]=0;
    //}
    emit RefreshView(1, "1 - OFFLINE");
    emit RefreshView(2, "2 - OFFLINE");
    emit RefreshView(3, "3 - OFFLINE");
    emit RefreshView(4, "4 - OFFLINE");
    emit RefreshView(5, "5 - OFFLINE");
    emit RefreshView(6, "6 - OFFLINE");

    bus->sendStr("clr");
    //QString a("clr");
    //a[a.length()]='\n';
    //bus->serial->write(a.toLatin1());
}


void controller::speakTerm(int t)
{
    if ((t >40)||(t<-40)) return;
    player->add("/temper/temp.wav");
    QString s;  s = QString("/temper/%1.wav")
            .arg(t);
    player->add(s);
    //qDebug() << s;
}


void controller::speakTime()
{
    QTime cur = QTime::currentTime();
    QString s;  s = QString("/hours/%1.wav")
            .arg(cur.hour());
    player->add(s);
    //qDebug() << s;
    s = QString("/minutes/%1.wav")
                .arg(cur.minute());
    player->add(s);
    //qDebug() << s;
}


void controller::bud_action(int num, QString action)
{
    player->add("bud.wav");
    speakTime();
    bus->sendStr(action);
}


void controller::main_control()
{

}


void controller::maintain()
{
    QString s;  s = QString("temp%1;")
            .arg(termo->temper);
    sendDatagram(s);

}


void controller::sendToView(int sn)
{
    QString s;  s = QString("S/N:0%1; SETS:%2; GAS:%3; REB:%4; TEM:%5; HUM:%6; BUT:%7;")
            .arg(sn)
            .arg(bus->sets[sn])
            .arg(bus->stat[sn][0])
            .arg(bus->rebs[sn])
            .arg(bus->stat[sn][1])
            .arg(bus->stat[sn][2])
            .arg(bus->butt[sn]);
    emit RefreshView(sn, s);
}


void controller::processPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
        QString data = QString(datagram);
        if (data[data.length()-1]!='\n') data[data.length()] = '\n';
                emit toLog(data);
        if (data.contains("temp"))
        {
            speakTerm(termo->temper);
        }
        else
        if (data.contains("time"))
        {
            player->add("vremya.wav");
            speakTime();
        }
        else
        if (data.contains("bud"))
        {

        }
        else
        if (data.contains("setbud"))
        {

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
            bus->sendStr(data);
            //bus->serial->write(data.toLatin1());
    }
}


void controller::sendDatagram(QString str)
{
    QByteArray datagram = str.toLatin1();
    udpSocket->writeDatagram(datagram.data(), datagram.size(),
                                QHostAddress("127.0.0.1"), PORT_SEND);
    //udpSocket->writeDatagram(datagram.data(), datagram.size(),
   //                         QHostAddress::Broadcast, PORT_SEND);
}


controller::~controller()
{
    tem_thread.quit();
    tem_thread.wait();
    bus_thread.quit();
    bus_thread.wait();
    link_thread.quit();
    link_thread.wait();
    audio_thread.quit();
    audio_thread.wait();
}

