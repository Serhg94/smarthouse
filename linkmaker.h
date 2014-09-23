#ifndef LINKMAKER_H
#define LINKMAKER_H

#include <QObject>
#include <eventengine/eventengine.h>
#include <QString>
#include <QFile>

// парсер файла скриптов, создает связи Conditions-Event-Link-Action-Commands

Linktimer* makeLinksFromFile(QString name, IOconnector *conn);
Link* parseLink(QString str, IOconnector *conn);
Action* parseAction(QString str, IOconnector *conn);
Event* parseEvent(QString str, IOconnector *conn);
Command* parseCommand(QString str, IOconnector *conn);
Condition* parseCondition(QString str, IOconnector *conn);

#endif // LINKMAKER_H
