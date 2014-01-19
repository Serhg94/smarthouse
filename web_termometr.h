#ifndef WEB_TERMOMETR_H
#define WEB_TERMOMETR_H

#include <QWidget>
#include <QTimer>
#include <QProcess>

class web_termometr : public QWidget
{
    Q_OBJECT
public:
    explicit web_termometr(QWidget *parent = 0);
    QTimer *maint;
    QProcess *page;
    int temper;

signals:

public slots:
    void update();
    void recive_temp();
    void recive_temp(int t);

};

#endif // WEB_TERMOMETR_H
