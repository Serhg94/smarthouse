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
    int doCommand();

private:
    void speakTerm();
    void speakTime();

signals:

public slots:

};

#endif // SAYCOMMAND_H
