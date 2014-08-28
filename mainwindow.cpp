#include "mainwindow.h"
#include "ui_mainwindow.h"

#define MAINTAIN_MSEC 30000

MainWindow::MainWindow(QApplication *a, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    app = a;
    maintain_timer = new QTimer(this);
    QObject::connect(maintain_timer, SIGNAL(timeout()), this, SLOT(maintain()));
    maintain_timer->start(MAINTAIN_MSEC);
    QObject::connect(ui->openButton, SIGNAL(clicked()), this, SLOT(portButt()));
    QObject::connect(ui->alarmButton, SIGNAL(clicked()), this, SLOT(alarmButt()));
    ui->listWidget->addItem(" ");
    ui->listWidget->addItem(" ");
    ui->listWidget->addItem(" ");
    ui->listWidget->addItem(" ");
    ui->listWidget->addItem(" ");
    ui->listWidget->addItem(" ");
    initPortBox();
    loadSettings();
}


void MainWindow::initPortBox()
{
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->portsBox->addItem(info.portName());
    }
}


void MainWindow::maintain()
{
    QTime cur = QTime::currentTime();

    ui->textBrowser->clear();

    //restart
    if ((cur.minute() == ui->restarttime->time().minute())&&(cur.hour() == ui->restarttime->time().hour())&&(ui->restartbox->isChecked()))
    {
        //qDebug()<<"11111111";
        qApp->quit();
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
        //qDebug()<<"22222222";
    }
}


void MainWindow::portButt()
{
    emit openPort(ui->portsBox->currentText());
}

void MainWindow::appendStr(QString info)
{
    ui->textBrowser->append(info);
}

void MainWindow::alarmButt()
{
    emit alarmWindowOpen();
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
}



void MainWindow::RefreshView(int sn, QString info)
{
    ui->listWidget->item(sn-1)->setText(info);
}


MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}


