#ifndef VARCONDITION_H
#define VARCONDITION_H

#include <QObject>
#include <eventengine/condition.h>
#include <eventengine/variables/variables.h>


class varCondition : public Condition
{
    Q_OBJECT
public:
    explicit varCondition(QObject *parent = 0);
    int checkCondition(rc_bus *bus);
    //1-больше, 2-меньше, 3-равно, 4-не равно
    short int type;
    short int number;
    int value;
    variables *vars;
signals:

public slots:

};

#endif // VARCONDITION_H
