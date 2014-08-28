#ifndef ALARMDIALOG_H
#define ALARMDIALOG_H

#include <QDialog>
#include <QTimer>
#include <QSettings>

namespace Ui {
class alarmDialog;
}

class alarmDialog : public QDialog
{
    Q_OBJECT

public:
    explicit alarmDialog(QWidget *parent = 0);
    QString getComm1();
    QString getComm2();
    QString getComm3();
    QTimer *bud_timer; //таймер будильников
    ~alarmDialog();

signals:
    void Alarm(int sn, QString action);

public slots:
    void loadSettings();
    void saveSettings();
    void bud_action();

private:
    Ui::alarmDialog *ui;
};

#endif // ALARMDIALOG_H
