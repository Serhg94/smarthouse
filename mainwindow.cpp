#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStringListModel>
#include <QTimer>



#define READ_SYCLES 100000
#define MCT_MSEC 300
#define MAINTAIN_MSEC 30000
#define UPT_MSEC 5000
#define PORT_SEND 6666
#define PORT_LISTEN 6667
#define DOOR_ALARM_SEC 300
#define ZAMOK_ALARM_SEC 300
#define SAY_INFO_SEC 1800
#define DOOR_LIGHT_SEC 45


MainWindow::MainWindow(QApplication *a, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    app = a;
    //6666 - в него сервер отправляет
    //6667 - его сервер слушает
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(PORT_LISTEN, QUdpSocket::ShareAddress);

    da_enable = false;
    za_enable = false;
    info_enable = false;
    doorlight_enable = false;

    termo = new web_termometr();

    initPortBox();

    player = new audiosteck();
    player->start();

    bus = new rc_bus();
    budil = new alarmDialog(this);

    QObject::connect(ui->alarmButton, SIGNAL(clicked()), this->budil, SLOT(show()));
    QObject::connect(ui->openButton, SIGNAL(clicked()), this, SLOT(openPort()));
    QObject::connect(budil, SIGNAL(Alarm(int)), this, SLOT(bud_action(int)));
    QObject::connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
    QObject::connect(this->bus, SIGNAL(gettedString(QString)), ui->textBrowser, SLOT(append(QString)));
    QObject::connect(this->bus, SIGNAL(sendedString(QString)), ui->textBrowser, SLOT(append(QString)));
    QObject::connect(this->bus, SIGNAL(gettedString(QString)), this, SLOT(broadcastDatagram(QString)));
    QObject::connect(ui->pushButton, SIGNAL(clicked()), this->bus, SLOT(reopen()));
    QObject::connect(this->bus, SIGNAL(statsChanged(int)), this, SLOT(RefreshView(int)));

    // Unindented for quoting purposes:
    ui->listWidget->addItem("1 - OFFLINE");
    ui->listWidget->addItem("2 - OFFLINE");
    ui->listWidget->addItem("3 - OFFLINE");
    ui->listWidget->addItem("4 - OFFLINE");
    ui->listWidget->addItem("5 - OFFLINE");
    ui->listWidget->addItem("6 - OFFLINE");

    loadSettings();

    // подключение CELAC
    linkengine = makeLinksFromFile("scripts.txt", bus, player, termo);
    linkengine->start();

    up_timer = new QTimer(this);
    QObject::connect(up_timer, SIGNAL(timeout()), this, SLOT(update()));
    up_timer->start(UPT_MSEC);

    main_control_timer = new QTimer(this);
    QObject::connect(main_control_timer, SIGNAL(timeout()), this, SLOT(main_control()));
    main_control_timer->start(MCT_MSEC);

    maintain_timer = new QTimer(this);
    QObject::connect(maintain_timer, SIGNAL(timeout()), this, SLOT(maintain()));
    maintain_timer->start(MAINTAIN_MSEC);
}


void MainWindow::initPortBox()
{
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->portsBox->addItem(info.portName());
    }
}


void MainWindow::saveSettings()
{
    QSettings configs("SerhgSoft", "SmartHouse");
    configs.setValue("dattimeB", ui->dayTB->time());
    configs.setValue("dattimeE", ui->dayTE->time());
    configs.setValue("reboottime", ui->restarttime->time());
    configs.setValue("rebootena", ui->restartbox->isChecked());
    configs.setValue("sayinfoena", ui->sayinfoBox->isChecked());
    configs.setValue("ddnigthena", ui->ddnigthBox->isChecked());
    configs.setValue("dooralertena", ui->dooralertBox->isChecked());
    configs.setValue("lockalertena", ui->lockalertBox->isChecked());
    configs.setValue("doorlightena", ui->doorlightBox->isChecked());
    budil->saveSettings();
}


void MainWindow::loadSettings()
{
    QSettings configs("SerhgSoft", "SmartHouse");
    ui->dayTB->setTime(configs.value("dattimeB").toTime());
    ui->dayTE->setTime(configs.value("dattimeE").toTime());
    ui->restarttime->setTime(configs.value("reboottime").toTime());
    ui->restartbox->setChecked(configs.value("rebootena").toBool());
    ui->sayinfoBox->setChecked(configs.value("sayinfoena").toBool());
    ui->ddnigthBox->setChecked(configs.value("ddnigthena").toBool());
    ui->dooralertBox->setChecked(configs.value("dooralertena").toBool());
    ui->lockalertBox->setChecked(configs.value("lockalertena").toBool());
    ui->doorlightBox->setChecked(configs.value("doorlightena").toBool());
    budil->loadSettings();
}


void MainWindow::openPort()
{
    bus->serial->close();
    bus->open_port(NULL, ui->portsBox->currentText());
}


bool MainWindow::exist_online()
{
    if (
    (ui->listWidget->item(0)->text()==("1 - OFFLINE"))&&
    (ui->listWidget->item(1)->text()==("2 - OFFLINE"))&&
    (ui->listWidget->item(2)->text()==("3 - OFFLINE"))&&
    (ui->listWidget->item(3)->text()==("4 - OFFLINE"))&&
    (ui->listWidget->item(4)->text()==("5 - OFFLINE"))&&
    (ui->listWidget->item(5)->text()==("6 - OFFLINE")))
        {
            return false;
        }
    return true;
}


void MainWindow::update()
{
    //for(int i = 0; i<=10; i++)
    //{
    //    bus->stat[i][1]=0;
    //}
    ui->listWidget->item(0)->setText("1 - OFFLINE");
    ui->listWidget->item(1)->setText("2 - OFFLINE");
    ui->listWidget->item(2)->setText("3 - OFFLINE");
    ui->listWidget->item(3)->setText("4 - OFFLINE");
    ui->listWidget->item(4)->setText("5 - OFFLINE");
    ui->listWidget->item(5)->setText("6 - OFFLINE");
    QString a("clr");
    a[a.length()]='\n';
    bus->serial->write(a.toLatin1());
}


void MainWindow::speakTerm(int t)
{
    if ((t >40)||(t<-40)) return;
    player->add("/temper/temp.wav");
    QString s;  s = QString("/temper/%1.wav")
            .arg(t);
    player->add(s);
    //qDebug() << s;
}


void MainWindow::speakTime()
{
    QTime cur = QTime::currentTime();
    QString s;  s = QString("/hours/%1.wav")
            .arg(cur.hour());
    player->add(s);
    //qDebug() << s;
    s = QString("/minutes/%1.wav")
                .arg(cur.minute());
    player->add(s);
    //qDebug() << s;
}


void MainWindow::bud_action(int num)
{
    player->add("bud.wav");
    speakTime();
    if (num==1) bus->sendStr(budil->getComm1());
    if (num==2) bus->sendStr(budil->getComm2());
    if (num==3) bus->sendStr(budil->getComm3());
}


void MainWindow::main_control()
{
    QTime cur = QTime::currentTime();
    //АЛГОРИТМЫ ДЛЯ ТРЕТЬЕГО КОНТРОЛЛЕРА _ ПРИХОЖАЯ
    if ((ui->listWidget->item(2)->text()!=("3 - OFFLINE")))
    {
        //1 зависимость: выключаем ДД2 в прихожей ночью
        if ((cur<ui->dayTB->time())||(cur>ui->dayTE->time()))
        {
            if((bus->sets[3][9]=='1')&&(ui->ddnigthBox->isChecked())) bus->sendCommand(3, "222222222022222");
        //2 зависимость: предупреждаем о незакрытых двери,замке
            if(bus->butt[3][0]=='1')
            {
                if ((!da_enable)&&(ui->dooralertBox->isChecked()))
                {
                    door_alarm = cur;
                    da_enable = true;
                    player->add("sndMsg.wav");
                }
            }
            if (bus->butt[3][2]=='1')
            {
                if ((!za_enable)&&(ui->lockalertBox->isChecked()))
                {
                    zamok_alarm = cur;
                    za_enable = true;
                    player->add("sndMsg2.wav");
                }
            }
        }
        else
        {
            //1 зависимость: включаем ДД2 в прихожей днем
            if((bus->sets[3][9]=='0')&&(ui->ddnigthBox->isChecked())) bus->sendCommand(3, "222222222122222");
            //2 зависимость: говорим инфу днем когда открывается дверь
            if(bus->butt[3][0]=='1')
            {
                if ((!info_enable)&&(ui->sayinfoBox->isChecked()))
                {
                    info_alarm = cur;
                    info_enable = true;
                    player->add("vremya.wav");
                    speakTime();
                    speakTerm(termo->temper);
                }
            }
        }
        // 3 зависимость: если открывается дверь - включаем свет на 45 секунд
        if((bus->butt[3][0]=='1')&&(bus->sets[3][3]=='0'))
        {
            if ((!doorlight_enable)&&(ui->doorlightBox->isChecked()))
            {
                doorlight_alarm = cur;
                doorlight_enable = true;
                bus->sendCommand(3, "222122222222222");
            }
        }
        // занесение в БД событий событие открытия/закрытия двери
        if ((door_status == -1)&&(bus->butt[3][0]=='1')) door_status = 1;
        if ((door_status == -1)&&(bus->butt[3][0]=='0')) door_status = 0;
        if ((door_status == 0)&&(bus->butt[3][0]=='1'))
        {
            door_status = 1;
            MailSender *mail = new MailSender("127.0.0.1", "stats@cyber-labs.ru", "stats@cyber-labs.ru", "Door is OPEN", "Door is OPEN");
            mail->start();
        }
        if ((door_status == 1)&&(bus->butt[3][0]=='0'))
        {
            door_status = 0;
            MailSender *mail = new MailSender("127.0.0.1", "stats@cyber-labs.ru", "stats@cyber-labs.ru", "Door is CLOSED", "Door is CLOSED");
            mail->start();
        }
    }
}


void MainWindow::maintain()
{
    QTime cur = QTime::currentTime();
    if (!exist_online())
    {
        bus->reopen();
    }
    if (da_enable)
    {
        if (door_alarm.secsTo(cur)>DOOR_ALARM_SEC)
            da_enable = false;
    }
    if (za_enable)
    {
        if (zamok_alarm.secsTo(cur)>ZAMOK_ALARM_SEC)
            za_enable = false;
    }
    if (info_enable)
    {
        if (info_alarm.secsTo(cur)>SAY_INFO_SEC)
            info_enable = false;
    }
    if (doorlight_enable)
    {
        if (doorlight_alarm.secsTo(cur)>DOOR_LIGHT_SEC){
            bus->sendCommand(3, "222022222222222");
            doorlight_enable = false;
        }
    }
    ui->textBrowser->clear();

    QString s;  s = QString("temp%1;")
            .arg(termo->temper);
    broadcastDatagram(s);

    //restart
    if ((cur.minute() == ui->restarttime->time().minute())&&(cur.hour() == ui->restarttime->time().hour())&&(ui->restartbox->isChecked()))
    {
        //qDebug()<<"11111111";
        qApp->quit();
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
        //qDebug()<<"22222222";
    }
}


void MainWindow::RefreshView(int sn)
{
    QString s;  s = QString("S/N:0%1; SETS:%2; GAS:%3; REB:%4; TEM:%5; HUM:%6; BUT:%7;")
            .arg(sn)
            .arg(bus->sets[sn])
            .arg(bus->stat[sn][0])
            .arg(bus->rebs[sn])
            .arg(bus->stat[sn][1])
            .arg(bus->stat[sn][2])
            .arg(bus->butt[sn]);
    ui->listWidget->item(sn-1)->setText(s);
}


void MainWindow::processPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
        QString data = QString(datagram);
        if (data[data.length()-1]!='\n') data[data.length()] = '\n';
                ui->textBrowser->append(data);
        if (data.contains("temp"))
        {
            speakTerm(termo->temper);
        }
        else
        if (data.contains("time"))
        {
            player->add("vremya.wav");
            speakTime();
        }
        else
        if (data.contains("bud"))
        {

        }
        else
        if (data.contains("setbud"))
        {

        }
        else
        // посылаем состояния всех галок, с большой буквы - чтоб не путалось с запросами от других клиентов
        if (data.contains("getoptions"))
        {
            QString s;  s = QString("Getoptions%1%2%3%4%5")
                    .arg((int)ui->sayinfoBox->isChecked())
                    .arg((int)ui->ddnigthBox->isChecked())
                    .arg((int)ui->dooralertBox->isChecked())
                    .arg((int)ui->lockalertBox->isChecked())
                    .arg((int)ui->doorlightBox->isChecked());
            broadcastDatagram(s);
        }
        else
        // установка присланных состояний галок
        if (data.contains("options"))
        {
            if (data[7]=='1') ui->sayinfoBox->setChecked(true);
            else if (data[7]=='0') ui->sayinfoBox->setChecked(false);
            if (data[8]=='1') ui->ddnigthBox->setChecked(true);
            else if (data[8]=='0') ui->ddnigthBox->setChecked(false);
            if (data[9]=='1') ui->dooralertBox->setChecked(true);
            else if (data[9]=='0') ui->dooralertBox->setChecked(false);
            if (data[10]=='1') ui->lockalertBox->setChecked(true);
            else if (data[10]=='0') ui->lockalertBox->setChecked(false);
            if (data[11]=='1') ui->doorlightBox->setChecked(true);
            else if (data[11]=='0') ui->doorlightBox->setChecked(false);
        }
        else
        //qDebug()<<datagram.data();
            bus->serial->write(data.toLatin1());
    }
}


void MainWindow::broadcastDatagram(QString str)
{
    QByteArray datagram = str.toLatin1();
    udpSocket->writeDatagram(datagram.data(), datagram.size(),
                             QHostAddress::Broadcast, PORT_SEND);
}


MainWindow::~MainWindow()
{
    saveSettings();
    up_timer->stop();
    up_timer->~QTimer();
    bus->~rc_bus();
    delete ui;
}


