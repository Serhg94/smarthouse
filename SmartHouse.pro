#-------------------------------------------------
#
# Project created by QtCreator 2013-10-19T11:47:03
#
#-------------------------------------------------

QT       += core gui serialport network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SmartHouse
TEMPLATE = app

#CONFIG += release

SOURCES += main.cpp\
        mainwindow.cpp \
    rc_bus.cpp \
    alarmdialog.cpp \
    audiosteck.cpp \
    mail/emailaddress.cpp \
    mail/mimeattachment.cpp \
    mail/mimefile.cpp \
    mail/mimehtml.cpp \
    mail/mimeinlinefile.cpp \
    mail/mimemessage.cpp \
    mail/mimepart.cpp \
    mail/mimetext.cpp \
    mail/smtpclient.cpp \
    mail/quotedprintable.cpp \
    mail/mimemultipart.cpp \
    mail/mimecontentformatter.cpp \
    web_termometr.cpp \
    eventengine/event.cpp \
    eventengine/condition.cpp \
    eventengine/timecondition.cpp \
    eventengine/daycondition.cpp \
    eventengine/pincondition.cpp \
    eventengine/analogcondition.cpp \
    eventengine/action.cpp \
    eventengine/command.cpp \
    eventengine/buscommand.cpp \
    eventengine/saycommand.cpp \
    eventengine/playcommand.cpp \
    eventengine/mailcommand.cpp \
    eventengine/link.cpp \
    eventengine/linktimer.cpp \
    eventengine/variables/variables.cpp \
    linkmaker.cpp \
    controller.cpp \
    eventengine/variables/varcondition.cpp \
    eventengine/variables/varcommand.cpp

HEADERS  += mainwindow.h \
    rc_bus.h \
    alarmdialog.h \
    audiosteck.h \
    mail/emailaddress.h \
    mail/mimeattachment.h \
    mail/mimefile.h \
    mail/mimehtml.h \
    mail/mimeinlinefile.h \
    mail/mimemessage.h \
    mail/mimepart.h \
    mail/mimetext.h \
    mail/smtpclient.h \
    mail/SmtpMime \
    mail/quotedprintable.h \
    mail/mimemultipart.h \
    mail/mimecontentformatter.h \
    mail/sender.h \
    web_termometr.h \
    eventengine/event.h \
    eventengine/condition.h \
    eventengine/timecondition.h \
    eventengine/daycondition.h \
    eventengine/pincondition.h \
    eventengine/analogcondition.h \
    eventengine/eventengine.h \
    eventengine/action.h \
    eventengine/command.h \
    eventengine/buscommand.h \
    eventengine/saycommand.h \
    eventengine/playcommand.h \
    eventengine/mailcommand.h \
    eventengine/link.h \
    eventengine/linktimer.h \
    eventengine/variables/variables.h \
    linkmaker.h \
    controller.h \
    eventengine/variables/varcondition.h \
    eventengine/variables/varcommand.h

FORMS    += mainwindow.ui \
    alarmdialog.ui

RESOURCES +=
