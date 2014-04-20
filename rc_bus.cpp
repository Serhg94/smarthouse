#include <QMessageBox>
#include <QTime>
#include "rc_bus.h"

rc_bus::rc_bus(QObject *parent) :
    QObject(parent)
{
    init();
    serial = new QSerialPort();
    QObject::connect(this->serial, SIGNAL(readyRead()), this, SLOT(readAllData()));
    QObject::connect(this, SIGNAL(gettedString(QString)), this, SLOT(parseDataStr(QString)));
    open_port("Arduino Mega 2560", NULL);
}


void rc_bus::reopen()
{
    serial->close();
    open_port(NULL, portstr);
}


bool rc_bus::open_port(QString desport, QString nameport)
{
    try{
        //while(!serial->isOpen())
        {
            if (desport!=NULL)
            foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
                if ((info.description()==desport)) {
                   serial->setPort(info);
                }
            }
            if (nameport!=NULL) serial->setPortName(nameport);
            if (serial->open(QIODevice::ReadWrite)){
                QSerialPortInfo info(serial->portName());
                if (!serial->setBaudRate(250000)) {
                    qDebug() << "Set baud rate " <<  250000 << " error.";

                };

                if (!serial->setDataBits(QSerialPort::Data8)) {
                    qDebug() << "Set data bits " <<  QSerialPort::Data8 << " error.";

                }

                if (!serial->setParity(QSerialPort::NoParity)) {
                    qDebug() << "Set parity " <<  QSerialPort::NoParity << " error.";

                }

                if (!serial->setStopBits(QSerialPort::OneStop)) {
                    qDebug() << "Set stop bits " <<  QSerialPort::OneStop << " error.";

                }

                if (!serial->setFlowControl(QSerialPort::NoFlowControl)) {
                    qDebug() << "Set flow " <<  QSerialPort::NoFlowControl << " error.";

                }
                qDebug() << "Name        : " << info.portName();
                qDebug() << "Description : " << info.description();
                qDebug() << "Manufacturer: " << info.manufacturer();
                qDebug() << "Port opened!";
                QString a("clr");
                a[a.length()]='\n';
                serial->write(a.toLatin1());
            }
           if (!serial->isOpen())
           {
               QMessageBox::critical(NULL,QObject::tr("Ошибка"),tr("Не могу открыть порт! Ошибка:")+serial->errorString());
               return false;
           }
           else portstr = serial->portName();
           return true;
        }
    }
    catch(...)
    {
        qDebug() << "Port open FAIL!";
        return false;
    }
}


void rc_bus::init()
{
    for(int i=0; i<10; i++)
    {
        sets[i] = "000000000000000";
        butt[i] = "0000";
        rebs[i] = "0000";
    }
}


void rc_bus::sendCommand(int sn, QString string)
{
    QString s;  s = QString("0%1set%2") .arg(sn) .arg(string);
    s[s.length()] = '\n';
    serial->write(s.toLatin1());
}


void rc_bus::sendStr(QString string)
{
    string[string.length()] = '\n';
    serial->write(string.toLatin1());
    emit sendedString(string);
}


void rc_bus::changeState(int num_ctr, int num_set)
{
    QString a("222222222222222");
    if (sets[num_ctr].at(num_set)=='1')
        a[num_set]='0';
    else
        a[num_set]='1';
    sendCommand(num_ctr, a);
}


int rc_bus::checkString(QString string, int from)
{
    bool ok;
    int sn=string.toInt(&ok);
    if ((sn<=0)||(ok==false)) return 0;
    return sn;
}


void rc_bus::parseDataStr(QString string)
{
    bool change=false;
    //qDebug() << string;
    QString str(string);
    if (str.length()<31) return;
      int sn=checkString(str.mid(0,1), 0);
      if ((sn<=0)) return;
      //парсим массив настроек
      if (sets[sn]!=str.mid(2,15))
      {
           sets[sn]=str.mid(2,15);
           change=true;
      }
      //парсим датчик дыма
      str=str.mid(18,str.length());
      int pos=str.indexOf('/');
      if (pos==-1) return;
      //if (abs(stat[sn][0]-str.mid(0,pos).toInt())>ACCURACY )
      {
        stat[sn][0]=checkString(str.mid(0,pos), 0); //Serial.print(stat[sn][0]);
      //  change=true;
      }
      //парсим массив релейных кнопо
      str=str.mid(pos+1,str.length());
      pos=str.indexOf('/');
      if (pos==-1) return;
      if (rebs[sn]!=str.mid(0,4))
      {
        rebs[sn]=str.mid(0,4);
        change=true;
      }
      //парсим температуру
      str=str.mid(5,str.length());
      pos=str.indexOf('/');
      if (pos==-1) return;
      if (stat[sn][1]!=checkString(str.mid(0,pos),0))
      {
        stat[sn][1]=checkString(str.mid(0,pos), 0);
        change=true;
      }
      //парсим влажность
      str=str.mid(pos+1,str.length());
      pos=str.indexOf('/');
      if (pos==-1) return;
      if (stat[sn][2]!=checkString(str.mid(0,pos), 0))
      {
        stat[sn][2]=checkString(str.mid(0,pos), 0);
        change=true;
      }
      //парсим массив кнопок
      str=str.mid(pos+1,str.length());
      pos=str.indexOf('/');
      if (pos==-1) return;
      if (butt[sn]!=str.mid(0,4))
      {
        butt[sn]=str.mid(0,4);
        change=true;
      }
      str=str.mid(pos+3,str.length());
      if (str.length()-pos>10)
      {
          parseDataStr(str);
      }
      emit statsChanged(sn);
      if (change) emit statsChangedCheck(sn);
}


void rc_bus::readAllData()
{
    QString res = "";
    QString a = "";
    a.append(this->serial->readAll());
    //qDebug() << a;
   // QTime cur = QTime::currentTime();
    // qDebug() << cur << a;
    if ( (a.at(a.length()-1))=='\n' )
        if (buffer != NULL){
            res = *buffer+a;
            buffer->~QString();
            buffer = NULL;
        }
        else{
            res = a;
        }
    else{
        if (buffer!=NULL) qDebug() << "!";
        buffer = new QString(a);
        return;
    }
    emit gettedString(res);
}


rc_bus::~rc_bus()
{
    serial->close();
}
