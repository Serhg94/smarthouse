#ifndef MATHPARSER_H
#define MATHPARSER_H

#include <QObject>
#include <QDebug>
#include "token.h"

class IOconnector;

class MathExpression
{
private:
    QHash<QString, QString> operators;
    QList<Token> stek;
    QList<Token> output;
    bool _const;
    Token ParseToken(QString &input, bool &can_be_unar);
public:
    QHash<QString, int> operators_priority;
    MathExpression();
    int Parse(QString s);
    QString primal_str;
    bool isConst() {return _const;}
    double Calculate(bool &error, IOconnector *conn);
};

#endif // MATHPARSER_H
