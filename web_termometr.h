#ifndef WEB_TERMOMETR_H
#define WEB_TERMOMETR_H

#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class web_termometr : public QObject
{
    Q_OBJECT
public:
    explicit web_termometr(QObject *parent = 0);
    QTimer *maint;
    int temper;
    bool _debug;

private:
    QNetworkAccessManager *m_WebCtrl;

signals:

private slots:
    void fileDownloaded(QNetworkReply* pReply);
    void update();

public slots:
    void init();

};

#endif // WEB_TERMOMETR_H
