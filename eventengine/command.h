#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>
#include <rc_bus.h>

class Command : public QObject
{
    Q_OBJECT
public:
    explicit Command(QObject *parent = 0);
    virtual int doCommand(rc_bus *bus);
signals:

public slots:

};

#endif // COMMAND_H
