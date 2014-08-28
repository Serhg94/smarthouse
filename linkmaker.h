#ifndef LINKMAKER_H
#define LINKMAKER_H

#include <QObject>
#include <eventengine/eventengine.h>
#include <QString>
#include <QFile>

// парсер файла скриптов, создает связи Conditions-Event-Link-Action-Commands

Linktimer* makeLinksFromFile(QString name, rc_bus *bus, audiosteck *player, web_termometr *termo, variables *var);
Link* parseLink(QString str, rc_bus *bus, audiosteck *player, web_termometr *termo, variables *var);
Action* parseAction(QString str, rc_bus *bus, audiosteck *player, web_termometr *termo, variables *var);
Event* parseEvent(QString str, rc_bus *bus, audiosteck *player, web_termometr *termo, variables *var);
Command* parseCommand(QString str, rc_bus *bus, audiosteck *player, web_termometr *termo, variables *var);
Condition* parseCondition(QString str, rc_bus *bus, audiosteck *player, web_termometr *termo, variables *var);

#endif // LINKMAKER_H
