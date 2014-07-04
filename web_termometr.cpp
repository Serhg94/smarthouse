#include "web_termometr.h"
#include <QDebug>

web_termometr::web_termometr(QWidget *parent)
{
    page = new QProcess();
    QObject::connect(page, SIGNAL(finished(int)), this, SLOT(recive_temp(int)));
    //QObject::connect(page, SIGNAL(readyRead()), this, SLOT(recive_temp()));
    page->start("temp_req.exe");
    maint = new QTimer();
    QObject::connect(maint, SIGNAL(timeout()), this, SLOT(update()));
    maint->start(120000);
}


void web_termometr::update()
{
    page->close();
    page->start("temp_req.exe");
}

void web_termometr::recive_temp()
{
    page->close();
    temper = page->readAll().toInt();
    //qDebug() << temper;
}

void web_termometr::recive_temp(int t)
{
    page->close();
    temper = t;
    //qDebug() << temper;
}
