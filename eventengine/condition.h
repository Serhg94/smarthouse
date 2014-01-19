#ifndef CONDITION_H
#define CONDITION_H

#include <QObject>
#include <rc_bus.h>

class Condition : public QObject
{
    Q_OBJECT
public:
    explicit Condition(QObject *parent = 0);
    virtual int checkCondition(rc_bus *bus);

signals:

public slots:

};

#endif // CONDITION_H
