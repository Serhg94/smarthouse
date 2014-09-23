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
    //1-присвоить, 2-плюс, 3-минус
    short int type;
    short int number;
    int value;
    int doCommand();
signals:

public slots:

};

#endif // VARCOMMAND_H
