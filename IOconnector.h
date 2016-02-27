#ifndef IOCONNECTOR_H
#define IOCONNECTOR_H

#include <QObject>
#include <QtCore/QDebug>
#include <QUdpSocket>
#include "rc_bus.h"
#include "audiosteck.h"
#include "sql_worker.h"
#include "web_termometr.h"
#include <variables/variables.h>
#include "config.h"


class IOconnector : public QObject
{
    Q_OBJECT
public:
    QUdpSocket *udpSocket;
    rc_bus *bus;
    variables *vars;
    audiosteck *player;
    web_termometr *termo;
    sql_worker *sql_db;
    explicit IOconnector(QObject *parent = 0);
    ~IOconnector();
private:
    QThread bus_thread;
    QThread audio_thread;
    QThread tem_thread;
    QThread sql_thread;

signals:

public slots:
    void bus_init(bool net, QString com, QString ip);

};

#endif // IOCONNECTOR_H
