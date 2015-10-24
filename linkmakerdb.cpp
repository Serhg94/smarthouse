
#include "linkmakerdb.h"
//#include <QMessageBox>

int makeLinksFromDB(IOconnector *conn, Linktimer *lt)
{
    try
    {
        QSqlQuery query("SELECT * FROM link");
        //qDebug() << query.lastError().number();
        while (query.next())
        {
            if (query.value("enabled").toBool())
            {
                Link *link = parseLink(query.value("idlink").toInt(), conn);
                link->description = query.value("description").toString();
                link->event_gen_onend = query.value("event_gen_onend").toBool();
                link->event_gen_onstart = query.value("event_gen_onstart").toBool();
                link->idlink = query.value("idlink").toInt();
                QString opt = query.value("option").toString();
                if (opt.contains("doafter", Qt::CaseInsensitive))
                    link->setDoAfter();
                if (opt.contains("oncecheck", Qt::CaseInsensitive))
                    link->setDoAfterOnceCheck();
                link->timeout = query.value("param").toInt();
                link->io_connector = conn;
                lt->links.append(link);
            }
        }
        qDebug()<<" Скрипты загружены";
        return 1;
    }
    catch(...)
    {
        qDebug()<<" Ошибка разбора файла скриптов";
//        QMessageBox::critical(NULL,QObject::tr("Ошибка"),QObject::tr("Ошибка разбора файла скриптов!"));
    }
    return 0;
}

Link* parseLink(int idlink, IOconnector *conn)
{
    Link *lk = new Link();
    if (idlink>=0)
    {
        lk->action = parseAction(idlink, conn);
        lk->event = parseEvent(idlink, conn);
    }
    return lk;
}

Action* parseAction(int idlink, IOconnector *conn)
{
    //qDebug() << str;
    Action *ac = new Action();
    //QString s;  s = QString("SELECT * FROM `action` WHERE `link_idlink`=%1")
    QString s;  s = QString("SELECT action.* FROM link_has_action LEFT JOIN action ON action.idaction = link_has_action.action_idaction WHERE link_has_action.link_idlink=%1 ORDER BY link_has_action.sort_index")
            .arg(idlink);
    QSqlQuery query(s);
    //qDebug() << query.lastError().number();
    while (query.next())
    {
        ac->commands.append(parseCommand(query.value("action").toString(),
                                         query.value("param").toString(),
                                         query.value("param1").toString(),
                                         query.value("param2").toString(),
                                         query.value("type").toString(),
                                         query.value("equipment_idequipment").toInt(), conn));
    }
    return ac;
}

Command* parseCommand(QString action, QString param, QString param1, QString param2, QString type, int id_equip, IOconnector *conn)
{
    //qDebug() << str;
    if (type.contains("saytime", Qt::CaseInsensitive))
    {
        sayCommand *cmd = new sayCommand();
        cmd->type = 1;
        cmd->io_connector = conn;
        return cmd;
    }
    else
    if (type.contains("saytemp", Qt::CaseInsensitive))
    {
        sayCommand *cmd = new sayCommand();
        cmd->type = 2;
        cmd->io_connector = conn;
        return cmd;
    }
    else
    if (type.contains("say", Qt::CaseInsensitive))
    {
        sayCommand *cmd = new sayCommand();
        cmd->speech = param;
        cmd->type = 3;
        //qDebug() << cmd->file;
        cmd->io_connector = conn;
        return cmd;
    }
    else
    if (type.contains("play", Qt::CaseInsensitive))
    {
        playCommand *cmd = new playCommand();
        cmd->file = param;
        //qDebug() << cmd->file;
        cmd->io_connector = conn;
        return cmd;
    }
    else
    if (type.contains("send", Qt::CaseInsensitive))
    {
        busCommand *cmd = new busCommand();
        cmd->comm = param;
        //qDebug() << cmd->comm;
        cmd->io_connector = conn;
        return cmd;
    }
    else
    if (type.contains("sql", Qt::CaseInsensitive))
    {
        sqlCommand *cmd = new sqlCommand();
        cmd->request = param;
        //qDebug() << cmd->comm;
        cmd->io_connector = conn;
        return cmd;
    }
    else
    if (type.contains("mail", Qt::CaseInsensitive))
    {
        mailCommand *cmd = new mailCommand();
        QString s;  s = QString("SELECT * FROM `equipment` WHERE `idequipment`=%1")
                .arg(id_equip);
        QSqlQuery query(s);
        //qDebug() << query.lastError().number();
        if (!query.next()) return NULL;
        cmd->server = query.value("addr").toString();

        cmd->sender = query.value("object").toString();

        cmd->to = param1;

        cmd->title = param2;

        cmd->body = param;
        cmd->io_connector = conn;
        return cmd;
    }
    else
    if (type.contains("var", Qt::CaseInsensitive))
    {
        QString s;  s = QString("SELECT * FROM `equipment` WHERE `idequipment`=%1")
                .arg(id_equip);
        QSqlQuery query(s);
        //qDebug() << query.lastError().number();
        if (!query.next()) return NULL;
        varCommand *cmd = new varCommand();
        QString str = action;
        if (str.indexOf("=")>-1) {cmd->type = 1;}
        else if (str.indexOf("+")>-1) {cmd->type = 2;}
        else if (str.indexOf("-")>-1) {cmd->type = 3;}
        cmd->number = query.value("addr").toInt();
        cmd->value = param.toInt();
        cmd->io_connector = conn;
        return cmd;
    }
    return NULL;
}

Event* parseEvent(int idlink, IOconnector *conn)
{
    //qDebug() << str;
    Event *ev = new Event();
    //QString s;  s = QString("SELECT * FROM `condition` WHERE `link_idlink`=%1")
    QString s;  s = QString("SELECT `condition`.* FROM link_has_condition LEFT JOIN `condition` ON `condition`.idcondition = link_has_condition.condition_idcondition WHERE link_has_condition.link_idlink=%1")
            .arg(idlink);
    QSqlQuery query(s);
    //qDebug() << query.lastError().number();
    while (query.next())
    {
        ev->conditions.append(parseCondition(query.value("condition").toString(),
                                         query.value("param").toString(),
                                         query.value("param1").toString(),
                                         query.value("param2").toString(),
                                         query.value("type").toString(),
                                         query.value("equipment_idequipment").toInt(), conn));
    }
    return ev;
}

Condition* parseCondition(QString condition, QString param, QString param1, QString param2, QString type, int id_equip, IOconnector *conn)
{
    //qDebug() << str;
    if (type.contains("time", Qt::CaseInsensitive))
    {
        timeCondition *cmd = new timeCondition();
        switch (condition.at(0).toLatin1())
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
        cmd->time = QTime(param.mid(0, param.indexOf(".")).toInt(), param.mid(param.indexOf(".")+1).toInt());
        //qDebug() << str.mid(str.indexOf(".")+1).toInt();
        cmd->io_connector = conn;
        return cmd;
    }
    else
    if (type.contains("day", Qt::CaseInsensitive))
    {
        dayCondition *cmd = new dayCondition();
        switch (condition.at(0).toLatin1())
        {
        case '=':
            cmd->type = 1;
            break;
        case '!':
            cmd->type = 2;
            break;
        }
        //qDebug() << str.mid(4).toInt();
        cmd->day = param.toInt();
        //qDebug() << str.mid(str.indexOf(".")+1).toInt();
        cmd->io_connector = conn;
        return cmd;
    }
    else
    if (type.contains("pin", Qt::CaseInsensitive))
    {
        QString s;  s = QString("SELECT * FROM `equipment` WHERE `idequipment`=%1")
                .arg(id_equip);
        QSqlQuery query(s);
        //qDebug() << query.lastError().number();
        if (!query.next()) return NULL;
        pinCondition *cmd = new pinCondition();
        cmd->pintype = query.value("pintype").toInt();
        int pos = -1;
        if (condition.indexOf(">1")>-1) {pos = condition.indexOf(">1"); cmd->type = 1;}
        if (condition.indexOf(">0")>-1) {pos = condition.indexOf(">0"); cmd->type = 2;}
        if (condition.indexOf("=1")>-1) {pos = condition.indexOf("=1"); cmd->type = 3;}
        if (condition.indexOf("=0")>-1) {pos = condition.indexOf("=0"); cmd->type = 4;}
        if (pos == -1) return NULL;
        cmd->pin = query.value("pinnumber").toInt();
        cmd->mk = query.value("controller").toInt();
        //qDebug() << (str.mid(11,pos-11)).toInt();
        cmd->io_connector = conn;
        return cmd;
    }
    else
    if (type.contains("analog", Qt::CaseInsensitive))
    {
        QString s;  s = QString("SELECT * FROM `equipment` WHERE `idequipment`=%1")
                .arg(id_equip);
        QSqlQuery query(s);
        //qDebug() << query.lastError().number();
        if (!query.next()) return NULL;
        analogCondition *cmd = new analogCondition();
        cmd->mk = query.value("controller").toInt();
        switch (condition.at(0).toLatin1())
        {
        case '>':
            cmd->type = 1;
            break;
        case '<':
            cmd->type = 2;
            break;
        }
        cmd->valtype = 0;
        cmd->value = (param.toInt());
        //qDebug() << cmd->value;
        cmd->io_connector = conn;
        return cmd;
    }
    else
    if (type.contains("tem", Qt::CaseInsensitive))
    {
        QString s;  s = QString("SELECT * FROM `equipment` WHERE `idequipment`=%1")
                .arg(id_equip);
        QSqlQuery query(s);
        //qDebug() << query.lastError().number();
        if (!query.next()) return NULL;
        analogCondition *cmd = new analogCondition();
        cmd->mk = query.value("controller").toInt();
        switch (condition.at(0).toLatin1())
        {
        case '>':
            cmd->type = 1;
            break;
        case '<':
            cmd->type = 2;
            break;
        }
        cmd->valtype = 1;
        cmd->value = (param.toInt());
        //qDebug() << cmd->value;
        cmd->io_connector = conn;
        return cmd;
    }
    else
    if (type.contains("hum", Qt::CaseInsensitive))
    {
        QString s;  s = QString("SELECT * FROM `equipment` WHERE `idequipment`=%1")
                .arg(id_equip);
        QSqlQuery query(s);
        //qDebug() << query.lastError().number();
        if (!query.next()) return NULL;
        analogCondition *cmd = new analogCondition();
        cmd->mk = query.value("controller").toInt();
        switch (condition.at(0).toLatin1())
        {
        case '>':
            cmd->type = 1;
            break;
        case '<':
            cmd->type = 2;
            break;
        }
        cmd->valtype = 2;
        cmd->value = (param.toInt());
        //qDebug() << cmd->value;
        cmd->io_connector = conn;
        return cmd;
    }
    else
    if (type.contains("var", Qt::CaseInsensitive))
    {
        QString s;  s = QString("SELECT * FROM `equipment` WHERE `idequipment`=%1")
                .arg(id_equip);
        QSqlQuery query(s);
        //qDebug() << query.lastError().number();
        if (!query.next()) return NULL;
        varCondition *cmd = new varCondition();
        int pos = -1;
        if (condition.indexOf(">")>-1) {pos = condition.indexOf(">"); cmd->type = 1;}
        if (condition.indexOf("<")>-1) {pos = condition.indexOf("<"); cmd->type = 2;}
        if (condition.indexOf("=")>-1) {pos = condition.indexOf("="); cmd->type = 3;}
        if (condition.indexOf("!")>-1) {pos = condition.indexOf("!"); cmd->type = 4;}
        if (pos == -1) return NULL;
        cmd->number = query.value("addr").toInt();
        cmd->value = (param.toInt());
        //qDebug() << cmd->value;
        //qDebug() << cmd->number;
        cmd->io_connector = conn;
        return cmd;
    }
    return NULL;
}
