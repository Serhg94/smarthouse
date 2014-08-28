#ifndef SENDER_H
#define SENDER_H

#include "mail/smtpmime"
#include "QThread"

class MailSender : public QThread
{

private:
    QString from, to, subject, body, server;
public:
    MailSender(const QString &server1, const QString &from1, const QString &to1, const QString &subject1, const QString &body1 )
    {
        server = server1;
        from = from1;
        to = to1;
        subject = subject1;
        body = body1;
    }

    void run ()
    {
        try
        {
            //Код, исполняемый в потоке
            SmtpClient smtp(server);
            //smtp.setUser("123@127.0.0.1");
           // smtp.setPassword("123");
            MimeMessage message;
            message.setSender(new EmailAddress(from, ""));
            message.addRecipient(new EmailAddress(to, ""));
            message.setSubject(subject);
            MimeText text;
            text.setText(body);
            message.addPart(&text);
            smtp.connectToHost();
            smtp.login();
            smtp.sendMail(message);
            smtp.quit();
            //this->deleteLater();
        }
        catch(...)
        {
            qDebug()<<" Ошибка отправки e-mail";
        }
    }

};




#endif // SENDER_H
