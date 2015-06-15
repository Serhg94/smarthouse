#ifndef WEB_TERMOMETR_H
#define WEB_TERMOMETR_H

#include <QMutex>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class web_termometr : public QObject
{
    Q_OBJECT
public:
    explicit web_termometr(QObject *parent = 0);
    volatile bool _debug;
    int get_T();

private:
    QMutex mutex;
    QNetworkAccessManager *m_WebCtrl;
    QTimer *maint;
    int temper;

signals:

private slots:
    void fileDownloaded(QNetworkReply* pReply);
    void update();

public slots:
    void init();

};

#endif // WEB_TERMOMETR_H
