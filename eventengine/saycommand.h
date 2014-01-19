#ifndef SAYCOMMAND_H
#define SAYCOMMAND_H

#include <eventengine/command.h>
#include "audiosteck.h"
#include "web_termometr.h"

class sayCommand : public Command
{
    Q_OBJECT
public:
    explicit sayCommand(QObject *parent = 0);
    // 1 - сказать время, 2 - сказать температуру
    short int type;

    audiosteck *pl;
    web_termometr *tem;
    int doCommand(rc_bus *bus);

private:
    void speakTerm();
    void speakTime();

signals:

public slots:

};

#endif // SAYCOMMAND_H
