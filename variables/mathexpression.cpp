#include "mathexpression.h"
#include <math.h>
#include <IOconnector.h>
#include "rc_bus.h"

MathExpression::MathExpression()
{
    operators_priority.insert("^",2);
    operators_priority.insert("*",4);
    operators_priority.insert("/",4);
    operators_priority.insert("+",6);
    operators_priority.insert("-",6);
    operators_priority.insert("(",8);
    operators_priority.insert(")",8);
    _const = true;
}

int MathExpression::Parse(QString str)
{
    stek.clear();
    output.clear();
    QString curr_str;
    Token next_token;
    Token last_t;
    bool can_be_unar = true;
    bool success = false;
    curr_str = str;
    while (curr_str.length()!= 0)
    {
        next_token = ParseToken(curr_str, can_be_unar);
        switch (next_token.GetType()) {
        case -1:
            return 0;
        case 0:
            output.append(next_token);
            break;
        case 3:
            if ((next_token.GetData()=="var")||(next_token.GetData()=="analog")
                    ||(next_token.GetData()=="temp")||(next_token.GetData()=="hum"))
                _const = false;
            stek.append(next_token);
            break;
        case 5:
            success = false;
            while (stek.size() != 0)
            {
                last_t = stek.last();
                if (last_t.GetData()=="(")
                {
                    success = true;
                    break;
                }
                output.append(last_t);
                stek.pop_back();
            }
            if (!success)
                return 0;
            break;
        case 1:
            while (stek.size() != 0)
            {
                last_t = stek.last();
                if (((last_t.GetType()==1)||(last_t.GetType()==2))&&
                        (operators_priority[next_token.GetData()]>=operators_priority[last_t.GetData()]))
                {
                    output.append(last_t);
                    stek.pop_back();
                }
                else
                    break;
            }
            stek.append(next_token);
            break;
        case 2:
            while (stek.size() != 0)
            {
                last_t = stek.last();
                if (((last_t.GetType()==1)||(last_t.GetType()==2))&&
                        (operators_priority[next_token.GetData()]>operators_priority[last_t.GetData()]))
                {
                    output.append(last_t);
                    stek.pop_back();
                }
                else
                    break;
            }
            stek.append(next_token);
            break;
        case 4:
            if (next_token.GetData()=="(")
            {
                stek.append(next_token);
                break;
            }
            success = false;
            while (stek.size() != 0)
            {
                last_t = stek.last();
                if (last_t.GetData()=="(")
                {
                    stek.pop_back();
                    success = true;
                    if (!stek.isEmpty())
                        if (stek.last().GetType()==3)
                        {
                            output.append(stek.last());
                            stek.pop_back();
                        }
                    break;
                }
                output.append(last_t);
                stek.pop_back();
            }
            if (!success)
                return 0;
            break;
        }
    }
    while (stek.size() != 0)
    {
        last_t = stek.last();
        if (last_t.GetType()==4)
        {
            return 0;
        }
        //qDebug() << last_t.GetData();
        output.append(last_t);
        stek.pop_back();
    }

    //for (int i =0; i<output.size(); i++)
    //    qDebug() << output[i].GetType() << output[i].GetData();
    primal_str = str;
    return 1;


        //-1 - ошибка
        //0 - число
        //1 - операция бинарная
        //2 - операция унарная
        //3 - функция
        //4 = скобка
        //5 = запятая
}


double MathExpression::Calculate(bool &error, IOconnector *conn)
{
    stek.clear();
    double second;
    double first;
    double result;
    QString oper;
    for (int i =0; i<output.size(); i++)
        switch (output[i].GetType()) {
        case 0:
            stek.append(output[i]);
            break;
        case 1:
            if (stek.size()<2)
            {
                error = true;
                return 0.0;
            }
            oper = output[i].GetData();
            second = stek.takeLast().GetData().toDouble();
            first = stek.takeLast().GetData().toDouble();
            if (oper == "+")
            {
                stek.append(Token(0, QString::number(first+second)));
            }
            else if (oper == "-")
            {
                stek.append(Token(0, QString::number(first-second)));
            }
            else if (oper == "*")
            {
                stek.append(Token(0, QString::number(first*second)));
            }
            else if (oper == "/")
            {
                stek.append(Token(0, QString::number(first/second)));
            }
            else if (oper == "^")
            {
                stek.append(Token(0, QString::number(pow(first,second))));
            }
            break;
        case 3:
            if (stek.size()<1)
            {
                error = true;
                return 0.0;
            }
            oper = output[i].GetData();
            result = 0.0;
            if (oper == "var")
            {
                first = stek.takeLast().GetData().toDouble();
                result = conn->vars->at(static_cast< int >(first));
                //qDebug() << result << output[i+1].GetData();
            }
            else if (oper == "analog")
            {
                //second = stek.takeLast().GetData().toDouble();
                first = stek.takeLast().GetData().toDouble();
                result = conn->bus->stat[static_cast< int >(first)][0];
            }
            else if (oper == "temp")
            {
                first = stek.takeLast().GetData().toDouble();
                result = conn->bus->stat[static_cast< int >(first)][1];
            }
            else if (oper == "hum")
            {
                first = stek.takeLast().GetData().toDouble();
                result = conn->bus->stat[static_cast< int >(first)][2];
            }
//            else if (oper == "analog2")
//            {
//                first = stek.takeLast().GetData().toDouble();
//                result = conn->bus->stat[static_cast< int >(first)][1]*256
//                        +conn->bus->stat[static_cast< int >(first)][2];
//            }
            else if (oper == "abs")
            {
                first = stek.takeLast().GetData().toDouble();
                result = abs(first);
            }
            else if (oper == "sqrt")
            {
                first = stek.takeLast().GetData().toDouble();
                result = sqrt(first);
            }
            if ((oper != "var")&&(oper != "abs")&&(oper != "sqrt")&&(result==-1))
                error = true;
            stek.append(Token(0, QString::number(result)));
            break;
        case 2:
            if (stek.size()<1)
            {
                error = true;
                return 0.0;
            }
            oper = output[i].GetData();
            first = stek.takeLast().GetData().toDouble();
            if (oper == "+")
            {
                stek.append(Token(0, QString::number(first)));
            }
            else if (oper == "-")
            {
                stek.append(Token(0, QString::number((-1)*first)));
            }
            break;
    }
    if (stek.size()!=1)
    {
        error = true;
        return 0.0;
    }
    else
        return stek.last().GetData().toDouble();
}



Token MathExpression::ParseToken(QString &input, bool &can_be_unar)
{
    int token_size = 0;
    int type = -1;
    QString data;
    //если число
    if (input.at(token_size).isDigit())
    {
        int dot_cnt = 0;
        // разрешаем только цифры и точку
        while (token_size < input.length() &&
               ((input.at(token_size).isDigit()) || input.at(token_size) == '.'))
        {
            // но также проверям, что в числе может быть только одна точка!
            if (input.at(token_size) == '.' && ++dot_cnt > 1) {
                return Token(-1, "");
            }
            token_size++;
        }
        data = input.mid(0, token_size);
        type = 0;
        can_be_unar = false;
    }
    else if (input.at(token_size).isLetter())
    {
        while (token_size < input.length() && (input.at(token_size).isLetter()))
            token_size++;
        data = input.mid(0, token_size);
        type = 3;
        can_be_unar = false;
    }
    else if ((input.at(token_size) == '+')||(input.at(token_size) == '-'))
    {
        data = input.mid(0, 1);
        if (can_be_unar)
            type = 2;
        else
            type = 1;
        can_be_unar = false;
        token_size++;
    }
    else if ((input.at(token_size) == '*')||(input.at(token_size) == '/')
             || (input.at(token_size) == '^'))
    {
        data = input.mid(0, 1);
        type = 1;
        can_be_unar = false;
        token_size++;
    }
    else if (input.at(token_size) == '(')
    {
        data = input.mid(0, 1);
        type = 4;
        can_be_unar = true;
        token_size++;
    }
    else if (input.at(token_size) == ')')
    {
        data = input.mid(0, 1);
        type = 4;
        can_be_unar = false;
        token_size++;
    }
    else if (input.at(token_size) == ',')
    {
        data = input.mid(0, 1);
        type = 5;
        can_be_unar = true;
        token_size++;
    }
    input = input.mid(token_size);
    return Token(type, data);
}






