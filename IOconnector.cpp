#include "IOconnector.h"

IOconnector::IOconnector(QObject *parent) :
    QObject(parent)
{
    //6668 - в него сервер отправляет
    //6669 - его сервер слушает
    udpSocket = new QUdpSocket();
    udpSocket->bind(PORT_LISTEN);

    termo = new web_termometr();
    QObject::connect(&tem_thread, SIGNAL(started()), termo, SLOT(init()));
    termo->moveToThread(&tem_thread);
    tem_thread.start();

    player = new audiosteck();
    QObject::connect(&audio_thread, SIGNAL(started()), player, SLOT(init()));
    player->moveToThread(&audio_thread);
    audio_thread.start();

    bus = new rc_bus(true);
    QObject::connect(&bus_thread, SIGNAL(started()), bus, SLOT(init()));
    bus->moveToThread(&bus_thread);
    bus_thread.start();

    sql_db = new sql_worker();
    QObject::connect(&sql_thread, SIGNAL(started()), sql_db, SLOT(init()));
    sql_db->moveToThread(&sql_thread);
    sql_thread.start();

    vars = new variables();
}

IOconnector::~IOconnector()
{
    tem_thread.quit();
    tem_thread.wait();
    bus_thread.quit();
    bus_thread.wait();
    audio_thread.quit();
    audio_thread.wait();
    sql_thread.quit();
    sql_thread.wait();
}
