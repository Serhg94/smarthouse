#include "sqlcommand.h"

sqlCommand::sqlCommand(QObject *parent) :
    Command(parent)
{
}

int sqlCommand::doCommand()
{
    io_connector->sql_db->addNonAnswerRequest(request);
    //if (bus->_debug)
        //qDebug() << " Bus send: " << comm;
    return 1;
}