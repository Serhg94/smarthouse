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
#include <eventengine/eventengine.h>
#include <QTimer>
#include "config.h"
#include "IOconnector.h"

class controller : public QObject
{
    Q_OBJECT
private:
    QThread link_thread;
    int many_thread;

public:
    IOconnector *io_connector;
    Linktimer *linkengine;
    QTimer *up_timer; //таймер обновления монитора контроллеров
    QTimer *maintain_timer; //таймер обслуживания - переподключение, сброс текста в окне


    explicit controller(QObject *parent = 0);
    bool exist_online();
    void initPortBox();
    void readConfig(QString name, bool *links_from_db);
    ~controller();

public slots:
    void init();
    void update();
    void openPort(QString port);
    void speakTime();
    void sendToView(int sn);
    void speakTerm(int t);
    void sendVariables();
    void appendStr(QString info);
    void processPendingDatagrams();
    void sendDatagram(QString str);
    void maintain();
signals:
    void toLog(QString info);

private slots:
    void _debugInfo(QString msg);
};

#endif // CONTROLLER_H
