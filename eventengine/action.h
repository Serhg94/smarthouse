#ifndef ACTION_H
#define ACTION_H

#include <QObject>
#include <eventengine/command.h>

class Action : public QObject
{
    Q_OBJECT
public:
    explicit Action(QObject *parent = 0);
    QVector <Command*> commands;
    int doAction();
signals:

public slots:

};

#endif // ACTION_H
