#ifndef SQL_WORKER_H
#define SQL_WORKER_H

#include <QObject>
#include <QDebug>
#include <QVector>
#include <QTimer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMutex>


class sql_worker : public QObject
{
    Q_OBJECT
public:
    explicit sql_worker(QObject *parent = 0);
    QString login;
    QString pass;
    QString addr;
    QString db_name;
    QString db_type;
    int port;
    QStringList makeLinksTableFromDB();

private:
    QTimer *maint;
    QVector <QString> requests;
    QMutex mutex;
    QSqlDatabase db;

signals:

public slots:
    void addNonAnswerRequest(QString req);
    void update();
    void init();
    void initDB();

};

#endif // SQL_WORKER_H
