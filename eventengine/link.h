#ifndef LINK_H
#define LINK_H

#include <QObject>
#include <eventengine/event.h>
#include <eventengine/action.h>
#include <QTimer>
#include <IOconnector.h>
#include <QTime>

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
    bool event_gen_onstart;
    bool event_gen_onend;
    QString description;
    QString dependences;
    int idlink;
    int timeout;
    void setDoAfter();
    void sendEvent();
    void setDoAfterOnceCheck();
    IOconnector * io_connector;
    int count;
    void checkStart();
    QTimer *timer;

private:
    bool doing;
    QTimer *do_after_timer;

signals:

public slots:
    void init_multy_thread();
    void enableLink();
    void init();
    void checkLink();
    void checkDepLink(QString depend);

};

#endif // LINK_H
