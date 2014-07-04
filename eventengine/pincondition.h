#ifndef PINCONDITION_H
#define PINCONDITION_H

#include <QObject>
#include <eventengine/condition.h>
#include <QString>

class pinCondition : public Condition
{
    Q_OBJECT

private:
    int pinState(rc_bus *bus);

public:
    explicit pinCondition(QObject *parent = 0);
    // 1- пин изменился на 1, 2 - пин изменился на 0, 3 - пин равен 1, 4 - пин равен 0.
    short int type;
    // 1- sets, 2 - rebs, 3 - butt
    short int pintype;

    short int pin;
    short int oldState;
    short int mk;
    // 1 - выполняется, 0 - не выполняется, 6 - что то пошло не так.
    int checkCondition(rc_bus *bus);
    void init(rc_bus *bus);
signals:

public slots:

};

#endif // PINCONDITION_H
