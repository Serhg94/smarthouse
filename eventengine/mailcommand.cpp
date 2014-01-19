#include "mailcommand.h"

mailCommand::mailCommand(QObject *parent) :
    Command(parent)
{
}

int mailCommand::doCommand(rc_bus *bus)
{
    MailSender *mail = new MailSender(server,
                                      sender,
                                      to, title,
                                      body);
    mail->start();
}
