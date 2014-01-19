#ifndef PLAYCOMMAND_H
#define PLAYCOMMAND_H

#include <eventengine/command.h>
#include "audiosteck.h"


class playCommand : public Command
{
    Q_OBJECT
public:
    explicit playCommand(QObject *parent = 0);
    // путь к файлу, который нужно проиграть
    QString file;

    audiosteck *pl;
    int doCommand(rc_bus *bus);

private:

signals:

public slots:

};


#endif // PLAYCOMMAND_H
