#ifndef LINK_H
#define LINK_H

#include <QObject>
#include <eventengine/event.h>
#include <eventengine/action.h>
#include <QTimer>

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
    void setDoAfter();
    void setDoAfterOnceCheck();

    void checkStart();
    QTimer *timer;

private:
    bool doing;
    QTimer *do_after_timer;

signals:

public slots:
    void enableLink();
    void init();
    void checkLink();

};

#endif // LINK_H
