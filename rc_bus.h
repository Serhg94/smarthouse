#ifndef RC_BUS_H
#define RC_BUS_H

#include <QtCore/QDebug>
#include <QObject>
#include <QVector>
#include <QTimer>
#include <QUdpSocket>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#define SEND_DELAY_MSEC 10
#define PORT_SEND1 6667
#define PORT_LISTEN1 6666

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

    bool _debug;

    explicit rc_bus(bool n = false, QObject *parent = 0);
    void run();
    int checkString(QString string, int from);
    void sendCommand(int sn, QString string);
    void sendStr(QString string);
    void preset();
    bool open_port(QString desport, QString nameport);

    QSerialPort *serial;
    ~rc_bus();

signals:
    void gettedString(QString str);
    void sendedString(QString str);
    void statsChanged(int sn);
    void statsChangedCheck(int sn);

public slots:
    void reopen();
    void init();
    void changeState(int num_ctr, int num_set);

private slots:
    void parseDataStr(QString string);
    void readAllData();
    void processPendingDatagrams();
    void send();

private:
    bool net;
    QString *buffer = NULL;
    QUdpSocket *udpSocket;
    QTimer *send_timer; //таймер между отправлениями команд из буфера
    QVector<QString> send_buff;
};

#endif // RC_BUS_H
