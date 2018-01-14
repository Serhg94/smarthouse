#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>

class IOconnector;

class Command : public QObject
{
    Q_OBJECT
public:
    IOconnector * io_connector;

    explicit Command(QObject *parent = 0);
    virtual int doCommand();
signals:

public slots:

};

#endif // COMMAND_H
