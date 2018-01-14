#ifndef ANALOGCONDITION_H
#define ANALOGCONDITION_H

#include <QObject>
#include <eventengine/condition.h>

class analogCondition : public Condition
{
    Q_OBJECT
public:
    explicit analogCondition(QObject *parent = 0);
    // тип сравнения: 1 - больше, 2 - меньше
    short int type;
    // значение чего сравниваем: 0 - аналоговый вход, 1 - температура, 2 - влажность 3 - состояние контроллера (онлайн = 1, оффлайн = -1)
    short int valtype;
    // номер контроллера
    short int mk;
    // значение
    int value;
    // 1 - выполняется, 0 - не выполняется, 6 - что то пошло не так.
    int checkCondition();

    QString getDependences(){return QString("analog%1").arg(mk);}
signals:

public slots:

};

#endif // ANALOGCONDITION_H
