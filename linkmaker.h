#ifndef LINKMAKER_H
#define LINKMAKER_H

#include <QObject>
#include <eventengine/eventengine.h>
#include <QString>
#include <QFile>

// парсер файла скриптов, создает связи Conditions-Event-Link-Action-Commands

Linktimer* makeLinksFromFile(QString name, rc_bus *bus, audiosteck *player, web_termometr *termo);
Link* parseLink(QString str, rc_bus *bus, audiosteck *player, web_termometr *termo);
Action* parseAction(QString str, rc_bus *bus, audiosteck *player, web_termometr *termo);
Event* parseEvent(QString str, rc_bus *bus, audiosteck *player, web_termometr *termo);
Command* parseCommand(QString str, rc_bus *bus, audiosteck *player, web_termometr *termo);
Condition* parseCondition(QString str, rc_bus *bus, audiosteck *player, web_termometr *termo);

#endif // LINKMAKER_H
