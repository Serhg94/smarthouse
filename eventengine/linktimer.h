#ifndef LINKTIMER_H
#define LINKTIMER_H

#include <QObject>
#include <eventengine/link.h>


class Linktimer : public QObject
{
    Q_OBJECT
public:
    explicit Linktimer(rc_bus *b, QObject *parent = 0);
    QVector <Link*> links;
    rc_bus *bus;
    QTimer *timer;
    void start();
signals:

public slots:
    void checkLinks();
};

#endif // LINKTIMER_H
