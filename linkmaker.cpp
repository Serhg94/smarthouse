#include "linkmaker.h"
#include <QMessageBox>

Linktimer* makeLinksFromFile(QString name, IOconnector *conn)
{
    try
    {
        QFile file(name);
        Linktimer *lt = new Linktimer();
        if (!file.open(QIODevice::ReadOnly)) // Проверяем, возможно ли открыть наш файл для чтения
            return lt; // если это сделать невозможно, то завершаем функцию
        char buf[2048];
        qint64 lineLength = file.readLine(buf, sizeof(buf));
        while(lineLength != -1)
        {
            QString str(buf);
            if (!str.contains("//"))
            {
                str = str.replace(" ", "");
                lt->links.append(parseLink(str, conn));
                //qDebug() << str;
            }
            lineLength = file.readLine(buf, sizeof(buf));
        }
        qDebug()<<" Скрипты загружены";
        return lt;
    }
    catch(...)
    {
        qDebug()<<" Ошибка разбора файла скриптов";
        QMessageBox::critical(NULL,QObject::tr("Ошибка"),QObject::tr("Ошибка разбора файла скриптов!"));
    }
}

Link* parseLink(QString str, IOconnector *conn)
{
    Link *lk = new Link();
    bool ok;
    if (str.lastIndexOf("timeout")==-1) return lk;
    int timeout = str.mid(str.lastIndexOf("timeout=")+8).toInt(&ok);
    if (str.mid(str.indexOf(";")).contains("doafter", Qt::CaseInsensitive))
        lk->setDoAfter();
    if (str.mid(str.indexOf(";")).contains("oncecheck", Qt::CaseInsensitive))
        lk->setDoAfterOnceCheck();
    if (ok) lk->timeout = timeout; else return lk;
    lk->action = parseAction(str.mid(str.indexOf("then")+4, str.indexOf(";")-str.indexOf("then")-4), conn);
    lk->event = parseEvent(str.mid(str.indexOf("if")+2, str.indexOf("then")-str.indexOf("if")-2), conn);
    //qDebug() << timeout;
    //qDebug() << str.mid(str.indexOf("if")+2, str.indexOf("then")-str.indexOf("if")-2);
    //qDebug() << str.mid(str.indexOf("then")+4, str.indexOf(";")-str.indexOf("then")-4);
    return lk;
}

Action* parseAction(QString str, IOconnector *conn)
{
    //qDebug() << str;
    Action *ac = new Action();
    while (str.indexOf("(")!=-1)
    {
        //qDebug() << str.mid(str.indexOf("(")+1, str.indexOf(")")-str.indexOf("(")-1);
        ac->commands.append(parseCommand(str.mid(str.indexOf("(")+1, str.indexOf(")")-str.indexOf("(")-1), conn));
        str = str.mid(str.indexOf(")")+1);
    }
    return ac;
}

Command* parseCommand(QString str, IOconnector *conn)
{
    //qDebug() << str;
    if (str.mid(0,7).contains("saytime", Qt::CaseInsensitive))
    {
        sayCommand *cmd = new sayCommand();
        cmd->type = 1;
        cmd->io_connector = conn;
        return cmd;
    }
    else
    if (str.mid(0,7).contains("saytemp", Qt::CaseInsensitive))
    {
        sayCommand *cmd = new sayCommand();
        cmd->type = 2;
        cmd->io_connector = conn;
        return cmd;
    }
    else
    if (str.mid(0,4).contains("play", Qt::CaseInsensitive))
    {
        playCommand *cmd = new playCommand();
        cmd->file = str.mid(str.indexOf("|")+1);
        //qDebug() << cmd->file;
        cmd->io_connector = conn;
        return cmd;
    }
    else
    if (str.mid(0,4).contains("send", Qt::CaseInsensitive))
    {
        busCommand *cmd = new busCommand();
        cmd->comm = str.mid(str.indexOf(".")+1);
        //qDebug() << cmd->comm;
        cmd->io_connector = conn;
        return cmd;
    }
    else
    if (str.mid(0,4).contains("mail", Qt::CaseInsensitive))
    {
        mailCommand *cmd = new mailCommand();
        cmd->server = str.mid(str.indexOf("|")+1,
                              str.indexOf("|", str.indexOf("|")+1)-str.indexOf("|")-1);
        str = str.mid(str.indexOf("|", str.indexOf("|")+1));

        cmd->sender = str.mid(str.indexOf("|")+1,
                              str.indexOf("|", str.indexOf("|")+1)-str.indexOf("|")-1);
        str = str.mid(str.indexOf("|", str.indexOf("|")+1));

        cmd->to = str.mid(str.indexOf("|")+1,
                              str.indexOf("|", str.indexOf("|")+1)-str.indexOf("|")-1);
        str = str.mid(str.indexOf("|", str.indexOf("|")+1));

        cmd->title = str.mid(str.indexOf("|")+1,
                              str.indexOf("|", str.indexOf("|")+1)-str.indexOf("|")-1);
        str = str.mid(str.indexOf("|", str.indexOf("|")+1));

        cmd->body = str.mid(str.indexOf("|")+1);
        cmd->io_connector = conn;
        return cmd;
    }
    else
    if (str.mid(0,3).contains("var", Qt::CaseInsensitive))
    {
        varCommand *cmd = new varCommand();
        int pos;
        if (str.indexOf("=")>-1) {pos = str.indexOf("="); cmd->type = 1;}
        if (str.indexOf("+")>-1) {pos = str.indexOf("+"); cmd->type = 2;}
        if (str.indexOf("-")>-1) {pos = str.indexOf("-"); cmd->type = 3;}
        cmd->number = (str.mid(4,pos-4)).toInt();
        cmd->value = (str.mid(pos+1).toInt());
        cmd->io_connector = conn;
        return cmd;
    }
}

Event* parseEvent(QString str, IOconnector *conn)
{
    //qDebug() << str;
    Event *ev = new Event();
    while (str.indexOf("(")!=-1)
    {
        //qDebug() << str.mid(str.indexOf("(")+1, str.indexOf(")")-str.indexOf("(")-1);
        ev->conditions.append(parseCondition(str.mid(str.indexOf("(")+1, str.indexOf(")")-str.indexOf("(")-1), conn));
        str = str.mid(str.indexOf(")")+1);
    }
    return ev;
}

Condition* parseCondition(QString str, IOconnector *conn)
{
    //qDebug() << str;
    if (str.mid(0,4).contains("time", Qt::CaseInsensitive))
    {
        timeCondition *cmd = new timeCondition();
        switch (str.at(4).toLatin1())
        {
        case '>':
            cmd->type = 1;
            break;
        case '<':
            cmd->type = 2;
            break;
        case '=':
            cmd->type = 3;
            break;
        }
        //qDebug() << cmd->type;
        cmd->time = QTime(str.mid(5, str.indexOf(".")-5).toInt(), str.mid(str.indexOf(".")+1).toInt());
        //qDebug() << str.mid(str.indexOf(".")+1).toInt();
        cmd->io_connector = conn;
        return cmd;
    }
    else
    if (str.mid(0,3).contains("day", Qt::CaseInsensitive))
    {
        dayCondition *cmd = new dayCondition();
        switch (str.at(3).toLatin1())
        {
        case '=':
            cmd->type = 1;
            break;
        case '!':
            cmd->type = 2;
            break;
        }
        //qDebug() << str.mid(4).toInt();
        cmd->day = str.mid(4).toInt();
        //qDebug() << str.mid(str.indexOf(".")+1).toInt();
        cmd->io_connector = conn;
        return cmd;
    }
    else
    if (str.mid(0,3).contains("pin", Qt::CaseInsensitive))
    {
        pinCondition *cmd = new pinCondition();
        //qDebug() << (str.mid(4,1).toInt());
        cmd->mk = (str.mid(4,1).toInt());
        if (str.mid(6,4)=="sets") cmd->pintype = 1;
        if (str.mid(6,4)=="rebs") cmd->pintype = 2;
        if (str.mid(6,4)=="butt") cmd->pintype = 3;
        //qDebug() << cmd->pintype;
        int pos;
        if (str.indexOf(">1")>-1) {pos = str.indexOf(">1"); cmd->type = 1;}
        if (str.indexOf(">0")>-1) {pos = str.indexOf(">0"); cmd->type = 2;}
        if (str.indexOf("=1")>-1) {pos = str.indexOf("=1"); cmd->type = 3;}
        if (str.indexOf("=0")>-1) {pos = str.indexOf("=0"); cmd->type = 4;}
        cmd->pin = (str.mid(11,pos-11)).toInt();
        //qDebug() << (str.mid(11,pos-11)).toInt();
        cmd->io_connector = conn;
        return cmd;
    }
    else
    if (str.mid(0,6).contains("analog", Qt::CaseInsensitive))
    {
        analogCondition *cmd = new analogCondition();
        cmd->mk = (str.mid(7,1).toInt());
        switch (str.at(8).toLatin1())
        {
        case '>':
            cmd->type = 1;
            break;
        case '<':
            cmd->type = 2;
            break;
        }
        cmd->valtype = 0;
        cmd->value = (str.mid(9).toInt());
        //qDebug() << cmd->value;
        cmd->io_connector = conn;
        return cmd;
    }
    else
    if (str.mid(0,3).contains("tem", Qt::CaseInsensitive))
    {
        analogCondition *cmd = new analogCondition();
        cmd->mk = (str.mid(4,1).toInt());
        switch (str.at(5).toLatin1())
        {
        case '>':
            cmd->type = 1;
            break;
        case '<':
            cmd->type = 2;
            break;
        }
        cmd->valtype = 1;
        cmd->value = (str.mid(6).toInt());
        //qDebug() << cmd->value;
        cmd->io_connector = conn;
        return cmd;
    }
    else
    if (str.mid(0,3).contains("hum", Qt::CaseInsensitive))
    {
        analogCondition *cmd = new analogCondition();
        cmd->mk = (str.mid(4,1).toInt());
        switch (str.at(5).toLatin1())
        {
        case '>':
            cmd->type = 1;
            break;
        case '<':
            cmd->type = 2;
            break;
        }
        cmd->valtype = 2;
        cmd->value = (str.mid(6).toInt());
        //qDebug() << cmd->value;
        cmd->io_connector = conn;
        return cmd;
    }
    else
    if (str.mid(0,3).contains("var", Qt::CaseInsensitive))
    {
        varCondition *cmd = new varCondition();
        int pos;
        if (str.indexOf(">")>-1) {pos = str.indexOf(">"); cmd->type = 1;}
        if (str.indexOf("<")>-1) {pos = str.indexOf("<"); cmd->type = 2;}
        if (str.indexOf("=")>-1) {pos = str.indexOf("="); cmd->type = 3;}
        if (str.indexOf("!")>-1) {pos = str.indexOf("!"); cmd->type = 4;}
        cmd->number = (str.mid(4,pos-4)).toInt();
        cmd->value = (str.mid(pos+1).toInt());
        //qDebug() << cmd->value;
        //qDebug() << cmd->number;
        cmd->io_connector = conn;
        return cmd;
    }
}
