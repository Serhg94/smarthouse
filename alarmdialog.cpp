#include <QtCore/QDebug>

#include "alarmdialog.h"
#include "ui_alarmdialog.h"
#define BUD_SEC 30000

alarmDialog::alarmDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::alarmDialog)
{
    ui->setupUi(this);
    bud_timer = new QTimer(this);
    QObject::connect(bud_timer, SIGNAL(timeout()), this, SLOT(bud_action()));
    bud_timer->start(BUD_SEC);
}

QString alarmDialog::getComm1()
{
    return ui->bud1comm->text();
}

QString alarmDialog::getComm2()
{
    return ui->bud2comm->text();
}

QString alarmDialog::getComm3()
{
    return ui->bud3comm->text();
}

void alarmDialog::saveSettings()
{
    QSettings configs("SerhgSoft", "SmartHouse");
    configs.setValue("bud1ena", ui->alcheckBox->isChecked());
    configs.setValue("bud2ena", ui->alcheckBox_2->isChecked());
    configs.setValue("bud3ena", ui->alcheckBox_3->isChecked());

    configs.setValue("bud1time", ui->altimeEdit->time());
    configs.setValue("bud2time", ui->altimeEdit_2->time());
    configs.setValue("bud3time", ui->altimeEdit_3->time());

    configs.setValue("pn1", ui->pn1->isChecked());
    configs.setValue("pn2", ui->pn2->isChecked());
    configs.setValue("pn3", ui->pn3->isChecked());

    configs.setValue("vt1", ui->vt1->isChecked());
    configs.setValue("vt2", ui->vt2->isChecked());
    configs.setValue("vt3", ui->vt3->isChecked());

    configs.setValue("sr1", ui->sr1->isChecked());
    configs.setValue("sr2", ui->sr2->isChecked());
    configs.setValue("sr3", ui->sr3->isChecked());

    configs.setValue("cht1", ui->cht1->isChecked());
    configs.setValue("cht2", ui->cht2->isChecked());
    configs.setValue("cht3", ui->cht3->isChecked());

    configs.setValue("pt1", ui->pt1->isChecked());
    configs.setValue("pt2", ui->pt2->isChecked());
    configs.setValue("pt3", ui->pt3->isChecked());

    configs.setValue("sb1", ui->sb1->isChecked());
    configs.setValue("sb2", ui->sb2->isChecked());
    configs.setValue("sb3", ui->sb3->isChecked());

    configs.setValue("vs1", ui->vs1->isChecked());
    configs.setValue("vs2", ui->vs2->isChecked());
    configs.setValue("vs3", ui->vs3->isChecked());

    configs.setValue("bud1str", ui->bud1comm->text());
    configs.setValue("bud2str", ui->bud2comm->text());
    configs.setValue("bud3str", ui->bud3comm->text());
}

void alarmDialog::loadSettings()
{
    QSettings configs("SerhgSoft", "SmartHouse");
    ui->alcheckBox->setChecked(configs.value("bud1ena").toBool());
    ui->alcheckBox_2->setChecked(configs.value("bud2ena").toBool());
    ui->alcheckBox_3->setChecked(configs.value("bud3ena").toBool());

    ui->altimeEdit->setTime(configs.value("bud1time").toTime());
    ui->altimeEdit_2->setTime(configs.value("bud2time").toTime());
    ui->altimeEdit_3->setTime(configs.value("bud3time").toTime());

    ui->pn1->setChecked(configs.value("pn1").toBool());
    ui->pn2->setChecked(configs.value("pn2").toBool());
    ui->pn3->setChecked(configs.value("pn3").toBool());

    ui->vt1->setChecked(configs.value("vt1").toBool());
    ui->vt2->setChecked(configs.value("vt2").toBool());
    ui->vt3->setChecked(configs.value("vt3").toBool());

    ui->sr1->setChecked(configs.value("sr1").toBool());
    ui->sr2->setChecked(configs.value("sr2").toBool());
    ui->sr3->setChecked(configs.value("sr3").toBool());

    ui->cht1->setChecked(configs.value("cht1").toBool());
    ui->cht2->setChecked(configs.value("cht2").toBool());
    ui->cht3->setChecked(configs.value("cht3").toBool());

    ui->pt1->setChecked(configs.value("pt1").toBool());
    ui->pt2->setChecked(configs.value("pt2").toBool());
    ui->pt3->setChecked(configs.value("pt3").toBool());

    ui->sb1->setChecked(configs.value("sb1").toBool());
    ui->sb2->setChecked(configs.value("sb2").toBool());
    ui->sb3->setChecked(configs.value("sb3").toBool());

    ui->vs1->setChecked(configs.value("vs1").toBool());
    ui->vs2->setChecked(configs.value("vs2").toBool());
    ui->vs3->setChecked(configs.value("vs3").toBool());

    ui->bud1comm->setText(configs.value("bud1str").toString());
    ui->bud2comm->setText(configs.value("bud2str").toString());
    ui->bud3comm->setText(configs.value("bud3str").toString());
}

void alarmDialog::bud_action()
{
    QTime curt = QTime::currentTime();
    QDate curd = QDate::currentDate();
    if (ui->alcheckBox->isChecked())
    {
        if ( ((curd.dayOfWeek()==1)&&(ui->pn1->isChecked())) ||
             ((curd.dayOfWeek()==2)&&(ui->vt1->isChecked())) ||
             ((curd.dayOfWeek()==3)&&(ui->sr1->isChecked())) ||
             ((curd.dayOfWeek()==4)&&(ui->cht1->isChecked())) ||
             ((curd.dayOfWeek()==5)&&(ui->pt1->isChecked())) ||
             ((curd.dayOfWeek()==6)&&(ui->sb1->isChecked())) ||
             ((curd.dayOfWeek()==7)&&(ui->vs1->isChecked())) )
        {
            if ((curt.hour()==ui->altimeEdit->time().hour())&&
                (curt.minute()==ui->altimeEdit->time().minute()))
            {
                //qDebug() << "БУДИЛЬНИК 1";
                emit Alarm(1);
            }
        }
    }
    if (ui->alcheckBox_2->isChecked())
    {
        if ( ((curd.dayOfWeek()==1)&&(ui->pn2->isChecked())) ||
             ((curd.dayOfWeek()==2)&&(ui->vt2->isChecked())) ||
             ((curd.dayOfWeek()==3)&&(ui->sr2->isChecked())) ||
             ((curd.dayOfWeek()==4)&&(ui->cht2->isChecked())) ||
             ((curd.dayOfWeek()==5)&&(ui->pt2->isChecked())) ||
             ((curd.dayOfWeek()==6)&&(ui->sb2->isChecked())) ||
             ((curd.dayOfWeek()==7)&&(ui->vs2->isChecked())) )
        {
            if ((curt.hour()==ui->altimeEdit_2->time().hour())&&
                (curt.minute()==ui->altimeEdit_2->time().minute()))
            {
                //qDebug() << "БУДИЛЬНИК 2";
                emit Alarm(2);
            }
        }
    }
    if (ui->alcheckBox_3->isChecked())
    {
        if ( ((curd.dayOfWeek()==1)&&(ui->pn3->isChecked())) ||
             ((curd.dayOfWeek()==2)&&(ui->vt3->isChecked())) ||
             ((curd.dayOfWeek()==3)&&(ui->sr3->isChecked())) ||
             ((curd.dayOfWeek()==4)&&(ui->cht3->isChecked())) ||
             ((curd.dayOfWeek()==5)&&(ui->pt3->isChecked())) ||
             ((curd.dayOfWeek()==6)&&(ui->sb3->isChecked())) ||
             ((curd.dayOfWeek()==7)&&(ui->vs3->isChecked())) )
        {
            if ((curt.hour()==ui->altimeEdit_3->time().hour())&&
                (curt.minute()==ui->altimeEdit_3->time().minute()))
            {
                //qDebug() << "БУДИЛЬНИК 3";
                emit Alarm(3);
            }
        }
    }
}


alarmDialog::~alarmDialog()
{
    delete ui;
}
