#include "web_termometr.h"
#include <QDebug>
#include <QXmlStreamReader>
#include <QNetworkRequest>

web_termometr::web_termometr(QObject *parent)
{
    _debug = false;
}

void web_termometr::init()
{
    m_WebCtrl = new QNetworkAccessManager();
    QObject::connect(m_WebCtrl, SIGNAL(finished(QNetworkReply*)),
                SLOT(fileDownloaded(QNetworkReply*)));
    maint = new QTimer();
    QObject::connect(maint, SIGNAL(timeout()), this, SLOT(update()));
    maint->start(120000);
    update();
}

int web_termometr::get_T()
{
    mutex.lock();
    int t = temper;
    mutex.unlock();
    return t;
}

void web_termometr::fileDownloaded(QNetworkReply* pReply)
{
    try
    {
        QString t(pReply->readAll());
        pReply->deleteLater();
        QXmlStreamReader xml(t);
        while (!xml.atEnd() && !xml.hasError())
        {
            QXmlStreamReader::TokenType token = xml.readNext();
            if (token == QXmlStreamReader::StartDocument)
                continue;
            if (token == QXmlStreamReader::StartElement)
            {
                if (xml.name() == "temperature"){
                    xml.readNext();
                    mutex.lock();
                    temper = xml.text().toInt();
                    mutex.unlock();
                    break;
                }
            }
        }
        if (_debug)
            qDebug() << " Temperatura = " << temper;
    }
    catch(...)
    {}
}


void web_termometr::update()
{
    QNetworkRequest request(QUrl ("http://export.yandex.ru/weather-ng/forecasts/27331.xml"));
    m_WebCtrl->get(request);
}
