#ifndef LINKTIMER_H
#define LINKTIMER_H

#include <QObject>
#include <QThread>
#include <eventengine/link.h>


class Linktimer : public QObject
{
    Q_OBJECT
public:
    explicit Linktimer(QObject *parent = 0);
    QVector <Link*> links;
    QTimer *timer;
    QVector <QThread*> link_threads;
signals:

public slots:
    void startInManyThreads();
    void startInOneThread();
    void checkLinks();
};

#endif // LINKTIMER_H
