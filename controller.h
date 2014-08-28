#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QThread>
#include <QtCore/QDebug>
#include <QUdpSocket>
#include <QSettings>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "rc_bus.h"
#include "audiosteck.h"
#include "web_termometr.h"
#include "mail/sender.h"
#include "linkmaker.h"
#include <eventengine/eventengine.h>
#include <QTimer>

class controller : public QObject
{
    Q_OBJECT
public:
    variables *vars;
    Linktimer *linkengine;
    QUdpSocket *udpSocket;
    QThread bus_thread;
    QThread link_thread;
    QThread audio_thread;
    QThread tem_thread;
    rc_bus *bus;
    audiosteck *player;
    web_termometr *termo;
    QTimer *up_timer; //таймер обновления монитора контроллеров
    QTimer *main_control_timer; //таймер контроля и управления (выключение включения света по ДД в прихожей)
    QTimer *maintain_timer; //таймер обслуживания - переподключение, сброс текста в окне


    explicit controller(QObject *parent = 0);
    bool exist_online();
    void initPortBox();
    ~controller();

public slots:
    void init();
    void update();
    void openPort(QString port);
    void speakTime();
    void sendToView(int sn);
    void speakTerm(int t);
    void appendStr(QString info);
    void bud_action(int num, QString action);
    void processPendingDatagrams();
    void sendDatagram(QString str);
    void main_control();
    void maintain();
signals:
    void RefreshView(int sn, QString info);
    void toLog(QString info);

public slots:

};

#endif // CONTROLLER_H
