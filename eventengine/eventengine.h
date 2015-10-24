//      Движок CELAC
// система связей Conditions-Event-Link-Action-Commands
// синтаксис
// if(time>12.30)(day=2)(pin.3.sets.3>1)(analog.3<400)then(send.It's Work!)(saytemp)(saytime)(mail|server|sender|toto|title|body);timeout=5000

#ifndef EVENTENGINE_H
#define EVENTENGINE_H

#include <eventengine/event.h>
#include <eventengine/timecondition.h>
#include <eventengine/analogcondition.h>
#include "IOconnector.h"

// type: 1- пин изменился на 1, 2 - пин изменился на 0, 3 - пин равен 1, 4 - пин равен 0.
// pintype: 1- sets, 2 - rebs, 3 - butt
// pin - номер пина в массиве пинтупе
// mk - номер мк
#include <eventengine/pincondition.h>
#include <eventengine/daycondition.h>

#include <eventengine/action.h>
#include <eventengine/command.h>
#include <eventengine/buscommand.h>
#include <eventengine/sqlcommand.h>
#include <eventengine/saycommand.h>
#include <eventengine/playcommand.h>
#include <eventengine/mailcommand.h>
#include <eventengine/variables/variables.h>
#include <eventengine/variables/varcommand.h>
#include <eventengine/variables/varcondition.h>

#include <eventengine/link.h>
#include <eventengine/linktimer.h>


#endif // EVENTENGINE_H
