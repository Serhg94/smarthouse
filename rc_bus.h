#ifndef RC_BUS_H
#define RC_BUS_H

#include <QVector>
#include <QMutex>
#include <QTimer>
#include <QUdpSocket>
#include <QtSerialPort/QSerialPort>


class rc_bus : public QObject
{
    Q_OBJECT

public:
    explicit rc_bus(bool n = false, QObject *parent = 0);

    // stat[num][3] - в сети ли контроллер
    int stat[10][10];
    QString sets[10];
    QString butt[10];
    QString rebs[10];
    QString portstr;
    QHostAddress ip;
    QMutex read_mutex;
    volatile bool _debug;
    unsigned int com_wait_answer_timeout;
    unsigned int com_poll_delay;

    void run();
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

private slots:
    void endCheck();
    void parseDataStr(QString string);
    void processPendingDatagrams();
    void send();
    void requestStatus();

private:
    QMutex send_mutex;
    bool net;
    QUdpSocket *udpSocket;
    QTimer *send_timer; //таймер между отправлениями команд из буфера
    QTimer *poll_states_timer; //таймер опроса шины
    int curr_poll_num;
    QTimer *reconnect_timer;
    QVector<QString> send_buff;

    int checkSet(int sn, QString sen);
    int checkString(QString string, int from);
};

#endif // RC_BUS_H
