#ifndef LINKTIMER_H
#define LINKTIMER_H

#include <QObject>
#include <QThread>
#include <eventengine/link.h>


class Linktimer : public QObject
{
    Q_OBJECT
public:
    explicit Linktimer(rc_bus *b, QObject *parent = 0);
    QVector <Link*> links;
    rc_bus *bus;
    QTimer *timer;
    QVector <QThread*> link_threads;
signals:

public slots:
    void startInManyThreads();
    void startInOneThread();
    void checkLinks();
};

#endif // LINKTIMER_H
