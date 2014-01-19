#ifndef RC_BUS_H
#define RC_BUS_H

#include <QtCore/QDebug>
#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>\


class rc_bus : public QObject
{
    Q_OBJECT

public:
    int stat[10][10];
    QString sets[10];
    QString butt[10];
    QString rebs[10];
    QString portstr;

    explicit rc_bus(QObject *parent = 0);
    int checkString(QString string, int from);
    void sendCommand(int sn, QString string);
    void sendStr(QString string);
    void init();
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
    void changeState(int num_ctr, int num_set);

private slots:
    void parseDataStr(QString string);
    void readAllData();

private:
    QString *buffer = NULL;
};

#endif // RC_BUS_H
