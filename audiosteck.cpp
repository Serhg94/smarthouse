#include "audiosteck.h"

audiosteck::audiosteck(QWidget *parent)
{
}

void audiosteck::init()
{
    player = new QMediaPlayer(this);
    maint = new QTimer(this);
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
        mutex2.lock();
        if ((!speechlist.isEmpty())&&(player->state()==QMediaPlayer::StoppedState))
        {
            QString text = speechlist.first();
            QString url_st = "http://translate.google.com/translate_tts?tl=ru&q=";
            url_st.append(QUrl::toPercentEncoding(text));
            speechlist.removeFirst();
            mutex2.unlock();
            player->setMedia(QUrl(url_st));
            player->play();
        }
        else mutex2.unlock();
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
    playlist << (QDir::cleanPath(QDir::currentPath()+QDir::separator()+name));
    mutex.unlock();
}

void audiosteck::addSpeech(QString text)
{
    mutex2.lock();
    if (speechlist.size()>=50) speechlist.clear();
    speechlist << text;
    mutex2.unlock();
}
