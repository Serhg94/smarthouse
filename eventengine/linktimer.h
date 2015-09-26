#ifndef LINKTIMER_H
#define LINKTIMER_H

#include <QObject>
#include <QThread>
#include <eventengine/link.h>
#include "IOconnector.h"


class Linktimer : public QObject
{
    Q_OBJECT
public:
    explicit Linktimer(IOconnector * io, QObject *parent = 0);
    QVector <Link*> links;
    QTimer *timer;
    QVector <QThread*> link_threads;
    IOconnector * io_connector;
signals:

public slots:
    void startInManyThreads();
    void startInOneThread();
    void checkLinks();
};

#endif // LINKTIMER_H
