#ifndef DAYCONDITION_H
#define DAYCONDITION_H

#include <QObject>
#include <eventengine/condition.h>
#include <QTime>

class dayCondition : public Condition
{
    Q_OBJECT
public:
    explicit dayCondition(QObject *parent = 0);
    // тип сравнения времени: 1 - равно, 2 - не равно
    short int type;
    // 1-7
    int day;
    // 1 - выполняется, 0 - не выполняется, 6 - что то пошло не так.
    int checkCondition(rc_bus *bus);
signals:

public slots:

};

#endif // DAYCONDITION_H
