#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QtCore/QDebug>
#include <QUdpSocket>
#include <QSettings>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "alarmdialog.h"
#include "rc_bus.h"
#include "audiosteck.h"
#include "web_termometr.h"
#include "mail/sender.h"
#include "linkmaker.h"
#include <eventengine/eventengine.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QTime door_alarm;
    QTime zamok_alarm;
    QTime info_alarm;
    QTime doorlight_alarm;
    bool da_enable;
    bool za_enable;
    bool info_enable;
    bool doorlight_enable;

    //логическое состояние двери
    int door_status;

    Linktimer *linkengine;
    QApplication *app;
    QUdpSocket *udpSocket;
    rc_bus *bus;
    audiosteck *player;
    web_termometr *termo;
    alarmDialog *budil;
    QTimer *up_timer; //таймер обновления монитора контроллеров
    QTimer *main_control_timer; //таймер контроля и управления (выключение включения света по ДД в прихожей)
    QTimer *maintain_timer; //таймер обслуживания - переподключение, сброс текста в окне
    explicit MainWindow(QApplication *a, QWidget *parent = 0);
    bool exist_online();
    void initPortBox();
    ~MainWindow();

public slots:
    void RefreshView(int);
    void update();
    void loadSettings();
    void saveSettings();
    void openPort();
    void speakTime();
    void speakTerm(int t);
    void bud_action(int num);
    void processPendingDatagrams();
    void sendDatagram(QString str);
    void main_control();
    void maintain();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
