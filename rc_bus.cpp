//#include <QMessageBox>
#include <QTime>
#include "rc_bus.h"

rc_bus::rc_bus(bool n, QObject *parent) :
    QObject(parent)
{
    net = n;
    _debug = false;
}

void rc_bus::init()
{
    if (net)
    {
        udpSocket = new QUdpSocket();
        udpSocket->bind(PORT_LISTEN1, QUdpSocket::ShareAddress);
        QObject::connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
        QObject::connect(this, SIGNAL(gettedString(QString)), this, SLOT(parseDataStr(QString)));
        send_timer = new QTimer();
        ip = QHostAddress::Broadcast;
        QObject::connect(send_timer, SIGNAL(timeout()), this, SLOT(send()));
        send_timer->start(SEND_DELAY_MSEC);
    }
    else
    {
        try
        {
            preset();
            serial = new QSerialPort();
            QObject::connect(serial, SIGNAL(readyRead()), this, SLOT(readAllData()));
            QObject::connect(this, SIGNAL(gettedString(QString)), this, SLOT(parseDataStr(QString)));
            open_port("Arduino Mega 2560", NULL);
            send_timer = new QTimer();
            QObject::connect(send_timer, SIGNAL(timeout()), this, SLOT(send()));
            send_timer->start(SEND_DELAY_MSEC);
        }
        catch(...)
        {
            qDebug() << " Port open FAIL!";
//            QMessageBox::critical(NULL,QObject::tr("Ошибка"),tr("Ошибка инициализации шины"));
        }
    }
    qDebug()<<" Порт открыт";
}


void rc_bus::reopen()
{
    if (!net)
    {
        serial->close();
        open_port(NULL, portstr);
    }
}


bool rc_bus::open_port(QString desport, QString nameport)
{
    if (!net)
    try{
        serial->close();
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
                if (!serial->setBaudRate(115200)) {
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
                //qDebug() << "Name        : " << info.portName();
                //qDebug() << "Description : " << info.description();
                //qDebug() << "Manufacturer: " << info.manufacturer();
                qDebug()<<" Порт открыт";
                QString a("clr");
                a[a.length()]='\n';
                serial->write(a.toLatin1());
            }
           if (!serial->isOpen())
           {
//               QMessageBox::critical(NULL,QObject::tr("Ошибка"),tr("Не могу открыть порт! Ошибка:")+serial->errorString());
               return false;
           }
           else portstr = serial->portName();
           return true;
        }
    }
    catch(...)
    {
        qDebug() << " Port open FAIL!";
    }
    return false;
}


void rc_bus::preset()
{
    read_mutex.lock();
    for(int i=0; i<10; i++)
    {
        sets[i] = "666666666666666";
        butt[i] = "6666";
        rebs[i] = "6666";
    }
    read_mutex.unlock();
}


void rc_bus::sendCommand(int sn, QString string)
{
    QString s = QString("0%1set%2") .arg(sn) .arg(string);
    send_mutex.lock();
    send_buff.append(s);
    send_mutex.unlock();
    //s[s.length()] = '\n';
    //serial->write(s.toLatin1());
}


void rc_bus::sendStr(QString string)
{
    send_mutex.lock();
    send_buff.append(string);
    send_mutex.unlock();
    //string[string.length()] = '\n';
    //serial->write(string.toLatin1());
    //qDebug()<<string.toLatin1();
    //emit sendedString(string);
}

void rc_bus::send()
{
    if (!net)
    {
        try
        {
            qDebug()<<"отправка";
            send_mutex.lock();
            if (send_buff.size()<1)
            {
                send_mutex.unlock();
                return;
            }
            QString string = send_buff.first();
            string[string.length()] = '\n';
            send_buff.pop_front();
            send_mutex.unlock();
            serial->write(string.toLatin1());
            emit sendedString(string);
        }
        catch(...)
        {
            qDebug()<<" Ошибка отправки команды в шину";
            //QMessageBox::critical(NULL,QObject::tr("Ошибка"),tr("Ошибка отправки команды в шину"));
        }
    }
    else
    {
        send_mutex.lock();
        if (send_buff.size()<1)
        {
            send_mutex.unlock();
            return;
        }
        QString string = send_buff.first();
        string[string.length()] = '\n';
        QByteArray datagram = string.toLatin1();
        send_buff.pop_front();
        send_mutex.unlock();
        udpSocket->writeDatagram(datagram.data(), datagram.size(),
                                    ip, PORT_SEND1);
        emit sendedString(string);
    }
}


void rc_bus::changeState(int num_ctr, int num_set)
{
    QString a("222222222222222");
    read_mutex.lock();
    if (sets[num_ctr].at(num_set)=='1')
        a[num_set]='0';
    else
        a[num_set]='1';
    read_mutex.unlock();
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
    try
    {
        bool change=false;
        //qDebug() << string;
        QString str(string);
        if (str.length()<31) return;
          int sn=checkString(str.mid(0,1), 0);
          if ((sn<=0)) return;
          //парсим массив настроек
          read_mutex.lock();
          if (sets[sn]!=str.mid(2,15))
          {
               sets[sn]=str.mid(2,15);
               change=true;
          }
          read_mutex.unlock();
          //парсим датчик дыма
          str=str.mid(18,str.length());
          int pos=str.indexOf('/');
          if (pos==-1) return;
          read_mutex.lock();
          //if (abs(stat[sn][0]-str.mid(0,pos).toInt())>ACCURACY )
          {
            stat[sn][0]=checkString(str.mid(0,pos), 0); //Serial.print(stat[sn][0]);
          //  change=true;
          }
          read_mutex.unlock();
          //парсим массив релейных кнопок
          str=str.mid(pos+1,str.length());
          pos=str.indexOf('/');
          if (pos==-1) return;
          read_mutex.lock();
          if (rebs[sn]!=str.mid(0,4))
          {
            rebs[sn]=str.mid(0,4);
            change=true;
          }
          read_mutex.unlock();
          //парсим температуру
          str=str.mid(5,str.length());
          pos=str.indexOf('/');
          if (pos==-1) return;
          read_mutex.lock();
          if (stat[sn][1]!=checkString(str.mid(0,pos),0))
          {
            stat[sn][1]=checkString(str.mid(0,pos), 0);
            change=true;
          }
          read_mutex.unlock();
          //парсим влажность
          str=str.mid(pos+1,str.length());
          pos=str.indexOf('/');
          if (pos==-1) return;
          read_mutex.lock();
          if (stat[sn][2]!=checkString(str.mid(0,pos), 0))
          {
            stat[sn][2]=checkString(str.mid(0,pos), 0);
            change=true;
          }
          read_mutex.unlock();
          //парсим массив кнопок
          str=str.mid(pos+1,str.length());
          pos=str.indexOf('/');
          if (pos==-1) return;
          read_mutex.lock();
          if (butt[sn]!=str.mid(0,4))
          {
            butt[sn]=str.mid(0,4);
            change=true;
          }
          read_mutex.unlock();
          str=str.mid(pos+3,str.length());
          if (str.length()-pos>10)
          {
              parseDataStr(str);
          }
          emit statsChanged(sn);
          if (change)
          {
              //read_mutex.tryLock();
              emit statsChangedCheck(sn);
          }
    }
    catch(...)
    {
        qDebug()<<"Ошибка разбора входящей строки";
        //QMessageBox::critical(NULL,QObject::tr("Ошибка"),tr("Ошибка отправки команды в шину"));
    }
}


void rc_bus::readAllData()
{
    try
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
    catch(...)
    {
        qDebug()<<" Ошибка чтения данных с шины";
        //QMessageBox::critical(NULL,QObject::tr("Ошибка"),tr("Ошибка чтения данных с шины"));
    }
}


void rc_bus::processPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size(), &ip);
        QString data = QString(datagram);
        emit gettedString(data);
    }
}


rc_bus::~rc_bus()
{
    if(!net)
        serial->close();
    qDebug()<<" Порт закрыт";
}
