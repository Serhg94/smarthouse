#ifndef VARCOMMAND_H
#define VARCOMMAND_H

#include <QObject>
#include <eventengine/command.h>
#include <eventengine/variables/variables.h>

class varCommand : public Command
{
    Q_OBJECT
public:
    explicit varCommand(QObject *parent = 0);
    variables *vars;
    //1-присвоить, 2-плюс, 3-минус
    short int type;
    short int number;
    int value;
    int doCommand(rc_bus *bus);
signals:

public slots:

};

#endif // VARCOMMAND_H
