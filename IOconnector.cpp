#include "IOconnector.h"

IOconnector::IOconnector(QObject *parent) :
    QObject(parent)
{
    //6668 - в него сервер отправляет
    //6669 - его сервер слушает
    udpSocket = new QUdpSocket();
    udpSocket->bind(PORT_LISTEN, QUdpSocket::ShareAddress);

    termo = new web_termometr();
    termo->moveToThread(&tem_thread);
    tem_thread.start();

    player = new audiosteck();
    player->moveToThread(&audio_thread);
    audio_thread.start();

    bus = new rc_bus(true);
    bus->moveToThread(&bus_thread);
    bus_thread.start();

    vars = new variables();

    QObject::connect(&tem_thread, SIGNAL(started()), termo, SLOT(init()));
    QObject::connect(&audio_thread, SIGNAL(started()), player, SLOT(init()));
    QObject::connect(&bus_thread, SIGNAL(started()), bus, SLOT(init()));
}

IOconnector::~IOconnector()
{
    tem_thread.quit();
    tem_thread.wait();
    bus_thread.quit();
    bus_thread.wait();
    audio_thread.quit();
    audio_thread.wait();
}
