#ifndef LINKMAKERDB_H
#define LINKMAKERDB_H

#include <QObject>
#include <eventengine/eventengine.h>
#include <QString>
#include <QSqlQuery>

// парсер файла скриптов, создает связи Conditions-Event-Link-Action-Commands

int makeLinksFromDB(IOconnector *conn, Linktimer *lt);
Link* parseLink(int idlink, IOconnector *conn);
Action* parseAction(int idlink, IOconnector *conn);
Command* parseCommand(QString action, QString param, QString param1, QString param2, QString type, int id_equip, IOconnector *conn);
Event* parseEvent(int idlink, IOconnector *conn);
Condition* parseCondition(QString condition, QString param, QString param1, QString param2, QString type, int id_equip, IOconnector *conn);

#endif // LINKMAKERDB_H
