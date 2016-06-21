#ifndef TIMECONDITION_H
#define TIMECONDITION_H

#include <QObject>
#include <eventengine/condition.h>
#include <QTime>

class timeCondition : public Condition
{
    Q_OBJECT
public:
    // тип сравнения времени: 1 - больше, 2 - меньше, 3 - равно
    short int type;
    QTime time;
    explicit timeCondition(QObject *parent = 0);
    // 1 - выполняется, 0 - не выполняется, 6 - что то пошло не так.
    int checkCondition();
    QString getDependences(){return QString("time");}
signals:

public slots:

};

#endif // TIMECONDITION_H
