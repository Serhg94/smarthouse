#ifndef MAILCOMMAND_H
#define MAILCOMMAND_H

#include <eventengine/command.h>
#include "mail/sender.h"

class mailCommand : public Command
{
    Q_OBJECT
public:
    explicit mailCommand(QObject *parent = 0);

    QString server, sender, to, title, body;
    int doCommand();
signals:

public slots:

};

#endif // MAILCOMMAND_H
