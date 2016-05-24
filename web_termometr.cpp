#include "web_termometr.h"
#include <QDebug>

//#include <QXmlStreamReader>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

web_termometr::web_termometr(QObject *parent)
{
    _debug = false;
}

void web_termometr::init()
{
    m_WebCtrl = new QNetworkAccessManager(this);
    QObject::connect(m_WebCtrl, SIGNAL(finished(QNetworkReply*)),
                SLOT(fileDownloaded(QNetworkReply*)));
    maint = new QTimer(this);
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
//        QXmlStreamReader xml(t);
//        while (!xml.atEnd() && !xml.hasError())
//        {
//            QXmlStreamReader::TokenType token = xml.readNext();
//            if (token == QXmlStreamReader::StartDocument)
//                continue;
//            if (token == QXmlStreamReader::StartElement)
//            {
//                if (xml.name() == "temperature"){
//                    xml.readNext();
//                    mutex.lock();
//                    temper = xml.text().toInt();
//                    mutex.unlock();
//                    break;
//                }
//            }
//        }

        QJsonDocument jsonResponse = QJsonDocument::fromJson(t.toUtf8());
        double temp = jsonResponse.object()["main"].toObject()["temp"].toDouble();
        mutex.lock();
        temper = temp - 273.15;
        mutex.unlock();

        if (_debug)
            qDebug() << " Temperatura = " << temper;
    }
    catch(...)
    {}
}


void web_termometr::update()
{
    //QNetworkRequest request(QUrl ("http://export.yandex.ru/weather-ng/forecasts/27331.xml"));
    QNetworkRequest request(QUrl (QString("http://api.openweathermap.org/data/2.5/weather?id=%1&appid=%2")
                                  .arg(sity).arg(api_key)));
    m_WebCtrl->get(request);
}
