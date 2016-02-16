#ifndef VARIABLES_H
#define VARIABLES_H

#include <QObject>
#include <QMutex>
#include <QTime>
#include <QVector>
#include <QList>
#include "config.h"
#include "mathexpression.h"


class variables : public QObject
{
    Q_OBJECT
private:
    QVector <double> vars;

public:
    QMutex mutex;
    QList <MathExpression*> value_generators;
    explicit variables(QObject *parent = 0);
    void changeValue(int num, double val);
    double at(int num);

signals:
    void valueChanged(int num, double val);

public slots:

};

#endif // VARIABLES_H
