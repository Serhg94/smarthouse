#ifndef BUSCOMMAND_H
#define BUSCOMMAND_H

#include <eventengine/command.h>

class busCommand : public Command
{
    Q_OBJECT
public:
    explicit busCommand(QObject *parent = 0);
    QString comm;
    int doCommand(rc_bus *bus);

signals:

public slots:

};

#endif // BUSCOMMAND_H
