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
        mutex.lock();
        if ((!playlist.isEmpty())&&(player->state()==QMediaPlayer::StoppedState))
        {
            QString name = playlist.first();
            playlist.removeFirst();
            mutex.unlock();
            player->setMedia(QUrl::fromLocalFile(name));
            player->play();
        }
        else mutex.unlock();
    }
    catch(...)
    {
        qDebug() << " Can't play file";
    }
}


void audiosteck::add(QString name)
{
    mutex.lock();
    if (playlist.size()>=50) playlist.clear();
    playlist << name;
    mutex.unlock();
}
