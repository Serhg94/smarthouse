#include "mailcommand.h"
#include <IOconnector.h>
#include "rc_bus.h"

mailCommand::mailCommand(QObject *parent) :
    Command(parent)
{
}

int mailCommand::doCommand()
{
    MailSender *mail = new MailSender(server,
                                      sender,
                                      to, title,
                                      body);
    if (io_connector->bus->_debug)
        qDebug() << " Send mail " << server << sender << to << title << body;
    mail->start();
    return 1;
}
