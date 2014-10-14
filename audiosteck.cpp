#include "audiosteck.h"


audiosteck::audiosteck(QWidget *parent)
{
}

void audiosteck::init()
{
    player = new QMediaPlayer();
    maint = new QTimer();
    QObject::connect(maint, SIGNAL(timeout()), this, SLOT(update()));
    maint->start(100);
}

void audiosteck::update()
{
    try
    {
        if ((!playlist.isEmpty())&&(player->state()==QMediaPlayer::StoppedState))
        {
            player->setMedia(QUrl::fromLocalFile(playlist.first()));
            player->play();
            playlist.removeFirst();
        }
    }
    catch(...)
    {
        qDebug() << " Can't play file";
    }
}


void audiosteck::add(QString name)
{
    if (playlist.size()>=50) playlist.clear();
    playlist << name;
}
