#ifndef SQLCOMMAND_H
#define SQLCOMMAND_H


#include <eventengine/command.h>

class sqlCommand : public Command
{
    Q_OBJECT
public:
    explicit sqlCommand(QObject *parent = 0);
    QString request;
    int doCommand();

signals:

public slots:

};

#endif // SQLCOMMAND_H
