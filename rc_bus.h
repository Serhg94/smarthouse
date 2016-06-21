#ifndef RC_BUS_H
#define RC_BUS_H

#include <QtCore/QDebug>
#include <QObject>
#include <QVector>
#include <QMutex>
#include <QTimer>
#include <QUdpSocket>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "config.h"


class rc_bus : public QObject
{
    Q_OBJECT

public:
    int stat[10][10];
    QString sets[10];
    QString butt[10];
    QString rebs[10];
    QString portstr;
    QHostAddress ip;
    QMutex read_mutex;
    volatile bool _debug;

    explicit rc_bus(bool n = false, QObject *parent = 0);
    void run();
    int checkString(QString string, int from);
    void sendCommand(int sn, QString string);
    void sendStr(QString string);
    void preset();
    bool open_port(QString desport, QString nameport);
    void setNetUse(bool _net) {net = _net;}
    bool getNetUse() {return net;}
    void initCheck();

    QSerialPort *serial;
    ~rc_bus();

signals:
    void gettedString(QString str);
    void sendedString(QString str);
    void statsChanged(int sn);
    void statsChangedCheck(QString depend);

public slots:
    void reopen();
    void init();
    void changeState(int num_ctr, int num_set);

private slots:
    void endCheck();
    void parseDataStr(QString string);
    void readAllData();
    void processPendingDatagrams();
    void send();

private:
    QMutex send_mutex;
    bool net;
    QString *buffer = NULL;
    QUdpSocket *udpSocket;
    QTimer *send_timer; //таймер между отправлениями команд из буфера
    QTimer *reconnect_timer;
    QVector<QString> send_buff;
};

#endif // RC_BUS_H
