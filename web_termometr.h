#ifndef WEB_TERMOMETR_H
#define WEB_TERMOMETR_H

#include <QWidget>
#include <QTimer>
#include <QProcess>

class web_termometr : public QObject
{
    Q_OBJECT
public:
    explicit web_termometr(QObject *parent = 0);
    QTimer *maint;
    QProcess *page;
    int temper;

signals:

public slots:
    void init();
    void update();
    void recive_temp();
    void recive_temp(int t);

};

#endif // WEB_TERMOMETR_H
