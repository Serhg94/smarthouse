#ifndef EVENT_H
#define EVENT_H

#include <QtCore/QDebug>
#include <QObject>
#include <eventengine/condition.h>

class Event : public QObject
{
    Q_OBJECT

public:
    QVector <Condition*> conditions;
    int checkEvent();
    explicit Event(QObject *parent = 0);

signals:


public slots:


private slots:


private:
    QString *buffer = NULL;
};

#endif // EVENT_H
