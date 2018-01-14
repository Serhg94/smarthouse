#ifndef CONDITION_H
#define CONDITION_H

#include <QObject>

class IOconnector;

class Condition : public QObject
{
    Q_OBJECT
public:
    IOconnector * io_connector;

    explicit Condition(QObject *parent = 0);
    virtual int checkCondition();
    virtual QString getDependences();

signals:

public slots:

};

#endif // CONDITION_H
