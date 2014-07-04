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
    bool enabled;
    bool do_after;
    bool once_check;
    int timeout;
    void checkLink(rc_bus *bus);
    void setDoAfter();
    void setDoAfterOnceCheck();

private:
    bool doing;
    QTimer *do_after_timer;

signals:

public slots:
    void enableLink();

};

#endif // LINK_H
