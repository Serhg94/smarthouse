#ifndef VARIABLES_H
#define VARIABLES_H

#include <QObject>
#include <eventengine/condition.h>
#include <eventengine/command.h>
#include <QTime>

#define VAR_COUNT 100

class variables : public QObject
{
    Q_OBJECT
public:
    explicit variables(QObject *parent = 0);
    QVector <int> vars;
    void changeValue(int num, int val);
signals:
    void valueChanged(int num, int val);

public slots:

};

#endif // VARIABLES_H
