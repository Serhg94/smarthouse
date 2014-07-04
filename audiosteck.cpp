#include "audiosteck.h"


audiosteck::audiosteck(QWidget *parent)
{
}

audiosteck::~audiosteck()
{
    qDebug() << "DESTRUCTOR";
}

void audiosteck::run()
{
    player = new QMediaPlayer();
    maint = new QTimer();
    QObject::connect(maint, SIGNAL(timeout()), this, SLOT(update()));
    maint->start(100);
    exec();
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
        qDebug() << QTime::currentTime().toString()+" Can't play file";
    }
}


void audiosteck::add(QString name)
{
    if (playlist.size()>=50) playlist.clear();
    playlist << name;
}
