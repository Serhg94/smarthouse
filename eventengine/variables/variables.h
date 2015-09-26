#ifndef VARIABLES_H
#define VARIABLES_H

#include <QObject>
#include <QMutex>
#include <QTime>
#include <QVector>
#include "config.h"

class variables : public QObject
{
    Q_OBJECT
private:
    QVector <int> vars;

public:
    QMutex mutex;
    explicit variables(QObject *parent = 0);
    void changeValue(int num, int val);
    int at(int num);
signals:
    void valueChanged(int num, int val);

public slots:

};

#endif // VARIABLES_H
