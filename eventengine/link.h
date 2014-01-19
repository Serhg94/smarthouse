#ifndef LINK_H
#define LINK_H

#include <QObject>
#include <eventengine/event.h>
#include <eventengine/action.h>
#include <QTimer>
#include "rc_bus.h"

class Link : public QObject
{
    Q_OBJECT
public:
    explicit Link(QObject *parent = 0);
    Event *event;
    Action *action;
    bool enabled = true;
    bool do_after = false;
    int timeout;
    void checkLink(rc_bus *bus);
    void setDoAfter();

private:
    bool doing = false;

signals:

public slots:
    void enableLink();

};

#endif // LINK_H
