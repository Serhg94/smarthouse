#include "sql_worker.h"

sql_worker::sql_worker(QObject *parent) : QObject(parent)
{
    login = "root";
    pass = "";
    port = 3306;
    addr = "localhost";
    db_name = "smarthouse";
    db_type = "QMYSQL";
}

void sql_worker::init()
{
    maint = new QTimer();
    QObject::connect(maint, SIGNAL(timeout()), this, SLOT(update()));
    maint->start(100);
    //qDebug() << "INIT" <<    this->thread()->currentThreadId();
}

void sql_worker::initDB()
{
//    if (db.isOpen())
//        return true;
    db = QSqlDatabase::addDatabase(db_type);
    db.setHostName(addr);
    db.setDatabaseName(db_name);
    db.setUserName(login);
    db.setPassword(pass);
    db.open();
}


QStringList sql_worker::makeLinksTableFromDB()
{
    //qDebug() << "MAKE" <<    this->thread()->currentThreadId();
    qDebug() << "d" <<db.isValid();
    QSqlQuery query("SELECT * FROM link");
    qDebug() << query.lastError().number();
    while (query.next()) {
        QString country = query.value(2).toString();
        qDebug() << country;
    }
    QStringList links;
    return links;
}


void sql_worker::update()
{
    try
    {
        //qDebug() << "update threaD" <<    this->thread()->currentThreadId();
        mutex.lock();
        if ((!requests.isEmpty()))
        {
            QString req = requests.first();
            requests.removeFirst();
            mutex.unlock();
            //DO REQUEST req
            //qDebug() <<req;
            QSqlQuery query(req, db);
            if (!db.isValid() ||query.lastError().number()==2006)
            {
                initDB();
                mutex.lock();
                addNonAnswerRequest(req);
                mutex.unlock();
            }
            //qDebug() <<req;
        }
        else mutex.unlock();
    }
    catch(...)
    {
        //qDebug() << " Can't do SQL request";
    }
}


void sql_worker::addNonAnswerRequest(QString name)
{
    mutex.lock();
    if (requests.size()>=50) requests.clear();
    requests << name;
    mutex.unlock();
}
