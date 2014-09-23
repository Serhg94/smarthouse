#ifndef CONDITION_H
#define CONDITION_H

#include <QObject>
#include <IOconnector.h>

class Condition : public QObject
{
    Q_OBJECT
public:
    IOconnector * io_connector;

    explicit Condition(QObject *parent = 0);
    virtual int checkCondition();

signals:

public slots:

};

#endif // CONDITION_H
