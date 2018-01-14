//#include <QMessageBox>
#include <QTime>
#include <QThread>
#include "rc_bus.h"
#include "binary.h"
#include <QtSerialPort/QSerialPortInfo>
#include "config.h"
#include <QtCore/QDebug>

const char end1 = B11111110;
const char end2 = B11111101;

rc_bus::rc_bus(bool n, QObject *parent) :
    QObject(parent)
{
    com_wait_answer_timeout = WAIT_ANSWER_TIMEOUT;
    com_poll_delay = POLL_DELAY_MSEC;
    net = n;
    ip = QHostAddress::Broadcast;
    _debug = false;
    for(int i=0; i<10; i++){
        for(int j=0; j<10; j++)
            stat[i][j]=-1;
    }
}

void rc_bus::init()
{
    if (net)
    {
        udpSocket = new QUdpSocket(this);
        udpSocket->bind(PORT_LISTEN1);
        QObject::connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
        QObject::connect(this, SIGNAL(gettedString(QString)), this, SLOT(parseDataStr(QString)));
        send_timer = new QTimer(this);
        QObject::connect(send_timer, SIGNAL(timeout()), this, SLOT(send()));
        send_timer->start(SEND_DELAY_MSEC);
        qDebug()<<" Setting connection wia LAN";
        sendStr("clr");
    }
    else
    {
        try
        {
            preset();
            serial = new QSerialPort(this);
            send_timer = new QTimer(this);
            reconnect_timer = new QTimer(this);
            poll_states_timer = new QTimer(this);
            QObject::connect(reconnect_timer, SIGNAL(timeout()), this, SLOT(reopen()));
            QObject::connect(poll_states_timer, SIGNAL(timeout()), this, SLOT(requestStatus()));
           // QObject::connect(send_timer, SIGNAL(timeout()), this, SLOT(send()));
            qDebug()<<" Setting connection wia COM port";
            if (!open_port(QString(), portstr))
                qDebug() << " Cant open COM port!";
           // send_timer->start(SEND_DELAY_MSEC);
            poll_states_timer->start(com_poll_delay);
            curr_poll_num = 1;
            reconnect_timer->start(COM_REOPEN_DELAY_MSEC);
        }
        catch(...)
        {
            qDebug() << " Cant open COM port!";
//            QMessageBox::critical(NULL,QObject::tr("Ошибка"),tr("Ошибка инициализации шины"));
        }
    }
}

void rc_bus::reopen()
{
    if (!serial->isOpen())
    {
        serial->close();
        if (!open_port(QString(), portstr))
            qDebug() << " Cant open COM port!";
    }
}

void rc_bus::initCheck()
{
    for(int j=0; j<10; j++)
        stat[j][3]=-1;
    if (net)
        sendStr("clr");
    emit sendedString("Refrash states");
    QTimer::singleShot(1000, this, SLOT(endCheck()));
}

void rc_bus::endCheck()
{
    for(int i=0; i<10; i++)
        if (stat[i][3]==-1)
        {
            read_mutex.lock();
            for(int j=0; j<10; j++)
                stat[i][j]=-1;
            sets[i] = "666666666666666";
            butt[i] = "6666";
            rebs[i] = "6666";
            read_mutex.unlock();
        }
}

bool rc_bus::open_port(QString desport, QString nameport)
{
    if (!net)
    try{
        serial->close();
        //while(!serial->isOpen())
        {
            if (!desport.isEmpty())
            foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
                if ((info.description()==desport)) {
                   serial->setPort(info);
                }
            }
            if (!nameport.isEmpty()) serial->setPortName(nameport);
            //qDebug() << nameport;
            if (serial->open(QIODevice::ReadWrite)){
                QSerialPortInfo info(serial->portName());
                if (!serial->setBaudRate(115200)) {
                    qDebug() << "Set baud rate " <<  115200 << " error.";
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
                qDebug()<<" Port opened";
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
}

QChar invers(QChar c){
    if (c=='1')
    return '0';
    return '1';
}

//1 - выполнено, 0- не выполнено
int rc_bus::checkSet(int sn, QString sen){
    if (((sen[0]=='2')||(sen[0]==sets[sn][0]))&&
    ((sen[1]=='2')||(sen[1]==sets[sn][1]))&&
    ((sen[2]=='2')||(sen[2]==sets[sn][2]))&&
    ((sen[3]=='2')||(sen[3]==sets[sn][3]))&&
    ((sen[4]=='2')||(sen[4]==sets[sn][4]))&&
    ((sen[5]=='2')||(sen[5]==sets[sn][5]))&&
    ((sen[6]=='2')||(sen[6]==sets[sn][6]))&&
    ((sen[7]=='2')||(sen[7]==sets[sn][7]))&&
    ((sen[8]=='2')||(sen[8]==sets[sn][8]))&&
    ((sen[9]=='2')||(sen[9]==sets[sn][9]))&&
    ((sen[10]=='2')||(sen[10]==sets[sn][10]))&&
    ((sen[11]=='2')||(sen[11]==sets[sn][11]))&&
    ((sen[12]=='2')||(sen[12]==sets[sn][12]))&&
    ((sen[13]=='2')||(sen[13]==sets[sn][13]))&&
    ((sen[14]=='2')||(sen[14]==sets[sn][14])))
        return 0;
    return 1;
}

void rc_bus::send()
{
    if (!net)
    {
        if (serial->isOpen())
        //    this->reopen();
        try
        {
            //qDebug()<<"отправка";
            send_mutex.lock();
            if (send_buff.size()<1)
            {
                send_mutex.unlock();
                return;
            }
            QString inputString = send_buff.first();
            send_buff.pop_front();
            send_mutex.unlock();
            int num = inputString.mid(0,2).toInt();
            if ((num<1)||(num>10)){
                qDebug() << " Wrong controller number in command";
                return;
            }
            bool check = true;
            QString sen = inputString.mid(5,inputString.length());
            sen.replace('\n', '2');
            while(sen.length()<15) sen+='2';
            for(int i=0; i<sen.length(); i++){
                if ((sen[i]!='1')&&(sen[i]!='0')&&(sen[i]!='2')&&(sen[i]!='3'))
                    check = false;
                if (sen[i]=='3')
                    sen[i]=invers(sets[num][i]);
            }
            if (!check){
                qDebug() << " Wrong command data";
                return;
            }
            if ((checkSet(num, sen)==1)&&(stat[num][3]!=-1)){
                QByteArray buf;
                buf.append((char)B00000000).append((char)B00000000).append((char)B00000000)
                        .append((char)B00000000).append((char)end1).append((char)end2);
                buf[0] = num;
                if ((sen[0] == '2')&&(sets[num][0]=='1')) buf[1] = buf[1]|B10000000;
                if ((sen[1] == '2')&&(sets[num][1]=='1')) buf[1] = buf[1]|B01000000;
                if ((sen[2] == '2')&&(sets[num][2]=='1')) buf[1] = buf[1]|B00100000;
                if ((sen[3] == '2')&&(sets[num][3]=='1')) buf[1] = buf[1]|B00010000;
                if ((sen[4] == '2')&&(sets[num][4]=='1')) buf[1] = buf[1]|B00001000;
                if ((sen[5] == '2')&&(sets[num][5]=='1')) buf[1] = buf[1]|B00000100;
                if ((sen[6] == '2')&&(sets[num][6]=='1')) buf[1] = buf[1]|B00000010;
                if ((sen[7] == '2')&&(sets[num][7]=='1')) buf[1] = buf[1]|B00000001;
                if ((sen[8] == '2')&&(sets[num][8]=='1')) buf[2] = buf[2]|B01000000;
                if ((sen[9] == '2')&&(sets[num][9]=='1')) buf[2] = buf[2]|B00100000;
                if ((sen[10] == '2')&&(sets[num][10]=='1')) buf[2] = buf[2]|B00010000;
                if ((sen[11] == '2')&&(sets[num][11]=='1')) buf[2] = buf[2]|B00001000;
                if ((sen[12] == '2')&&(sets[num][12]=='1')) buf[2] = buf[2]|B00000100;
                if ((sen[13] == '2')&&(sets[num][13]=='1')) buf[2] = buf[2]|B00000010;
                if ((sen[14] == '2')&&(sets[num][14]=='1')) buf[2] = buf[2]|B00000001;
                if ((sen[0] == '1')) buf[1] = buf[1]|B10000000;
                if ((sen[1] == '1')) buf[1] = buf[1]|B01000000;
                if ((sen[2] == '1')) buf[1] = buf[1]|B00100000;
                if ((sen[3] == '1')) buf[1] = buf[1]|B00010000;
                if ((sen[4] == '1')) buf[1] = buf[1]|B00001000;
                if ((sen[5] == '1')) buf[1] = buf[1]|B00000100;
                if ((sen[6] == '1')) buf[1] = buf[1]|B00000010;
                if ((sen[7] == '1')) buf[1] = buf[1]|B00000001;
                if ((sen[8] == '1')) buf[2] = buf[2]|B01000000;
                if ((sen[9] == '1')) buf[2] = buf[2]|B00100000;
                if ((sen[10] == '1')) buf[2] = buf[2]|B00010000;
                if ((sen[11] == '1')) buf[2] = buf[2]|B00001000;
                if ((sen[12] == '1')) buf[2] = buf[2]|B00000100;
                if ((sen[13] == '1')) buf[2] = buf[2]|B00000010;
                if ((sen[14] == '1')) buf[2] = buf[2]|B00000001;
                buf[3] = 0-(buf[0]+buf[1]+buf[2]);
                serial->write(buf);
                serial->waitForBytesWritten();
                emit sendedString("SENDED: "+inputString);
            }
        }
        catch(...)
        {
            qDebug()<<" Error while sending command to bus";
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
        emit sendedString("SENDED: "+string);
    }
}

void rc_bus::requestStatus()
{
    if (!this->serial->isOpen()){
        QThread::msleep(5);
        return;
    }
    QByteArray buf;
    buf[0]= curr_poll_num;
    buf[1]= curr_poll_num;
    buf[2]= curr_poll_num;
    buf[3]= curr_poll_num;
    buf[4]= end1;
    buf[5]= end2;
    this->serial->write(buf);
    this->serial->waitForBytesWritten();
    buf.clear();
    while (this->serial->waitForReadyRead(com_wait_answer_timeout))
        buf += this->serial->readAll();
    if (buf.size() == 12 &&(buf[buf.size()-2]==end1)&&(buf[buf.size()-1]==end2))
    {
        if (buf[0]==(char)curr_poll_num)
        if ((uint8_t)(0-(buf[0]+buf[1]+buf[2]+buf[3]+buf[4]+buf[5]+buf[6]+buf[7]+buf[8])) == (uint8_t)buf[9])
        {
            bool changed = false;
            QString str = "";
            if (buf[1]&B10000000) str += '1'; else str += '0';
            if (buf[1]&B01000000) str += '1'; else str += '0';
            if (buf[1]&B00100000) str += '1'; else str += '0';
            if (buf[1]&B00010000) str += '1'; else str += '0';
            if (buf[1]&B00001000) str += '1'; else str += '0';
            if (buf[1]&B00000100) str += '1'; else str += '0';
            if (buf[1]&B00000010) str += '1'; else str += '0';
            if (buf[1]&B00000001) str += '1'; else str += '0';
            if (buf[2]&B01000000) str += '1'; else str += '0';
            if (buf[2]&B00100000) str += '1'; else str += '0';
            if (buf[2]&B00010000) str += '1'; else str += '0';
            if (buf[2]&B00001000) str += '1'; else str += '0';
            if (buf[2]&B00000100) str += '1'; else str += '0';
            if (buf[2]&B00000010) str += '1'; else str += '0';
            if (buf[2]&B00000001) str += '1'; else str += '0';
            if (sets[curr_poll_num] != str) changed = true;
            sets[curr_poll_num] = str;
            stat[curr_poll_num][0] = 0;
            stat[curr_poll_num][0] += (unsigned char)buf[3] << 8;
            stat[curr_poll_num][0] += (unsigned char)buf[4];
            str = "";
            if (buf[5]&B10000000) str += '1'; else str += '0';
            if (buf[5]&B01000000) str += '1'; else str += '0';
            if (buf[5]&B00100000) str += '1'; else str += '0';
            if (buf[5]&B00010000) str += '1'; else str += '0';
            if (rebs[curr_poll_num] != str) changed = true;
            rebs[curr_poll_num] = str;
            stat[curr_poll_num][1] = (unsigned char)buf[6];
            stat[curr_poll_num][2] = (unsigned char)buf[7];
            str = "";
            if (buf[8]&B10000000) str += '1'; else str += '0';
            if (buf[8]&B01000000) str += '1'; else str += '0';
            if (buf[8]&B00100000) str += '1'; else str += '0';
            if (buf[8]&B00010000) str += '1'; else str += '0';
            if (butt[curr_poll_num] != str) changed = true;
            butt[curr_poll_num] = str;
            if (stat[curr_poll_num][3]!=1) changed = true;
            stat[curr_poll_num][3]=1;
            if (changed)
            {
                str = QString::number(curr_poll_num)+'/'+sets[curr_poll_num]+'/'+stat[curr_poll_num][0]+'/'+rebs[curr_poll_num]
                        +'/'+stat[curr_poll_num][1]+'/'+stat[curr_poll_num][2]+'/'+butt[curr_poll_num]+'/';
                emit gettedString(str);
                emit statsChanged(curr_poll_num);
                emit statsChangedCheck(QString("bus%1").arg(curr_poll_num));
            }
        }
    }
    else{
        stat[curr_poll_num][3]=-1;
        qDebug() << " No answer from conntroller number " << curr_poll_num;
    }
    curr_poll_num++;
    if (curr_poll_num > CONTROLLERS_COUNT)
        curr_poll_num = 1;
    send();
}

int rc_bus::checkString(QString string, int from)
{
    Q_UNUSED(from)
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
          stat[sn][3]=1;
          emit statsChanged(sn);
          if (change)
          {
              //read_mutex.tryLock();
              emit statsChangedCheck(QString("bus%1").arg(sn));
          }
    }
    catch(...)
    {
        qDebug()<<" Error while parse incoming data";
        //QMessageBox::critical(NULL,QObject::tr("Ошибка"),tr("Ошибка отправки команды в шину"));
    }
}

void rc_bus::processPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress new_ip;
        udpSocket->readDatagram(datagram.data(), datagram.size(), &new_ip);
        if (datagram.length()<3) return;
        if (ip==QHostAddress::Broadcast)
        {
            ip = new_ip;
            qDebug()<< " Main controller answered from address: " << ip.toString();
        }
        else if ((ip.toIPv4Address()!=new_ip.toIPv4Address())&&(ip!=new_ip))
            return;
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
