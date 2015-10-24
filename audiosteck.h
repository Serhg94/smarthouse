#ifndef AUDIOSTECK_H
#define AUDIOSTECK_H

#include <QtMultimedia>
#include <QMediaPlayer>
#include <QVector>
#include <QTimer>
#include <QMutex>

class audiosteck : public QObject
{
    Q_OBJECT
public:
    explicit audiosteck(QWidget *parent = 0);

private:
    QTimer *maint;
    QMediaPlayer *player;
    QVector <QString> playlist;
    QVector <QString> speechlist;
    QMutex mutex;
    QMutex mutex2;
signals:

public slots:
    void add(QString name);
    void addSpeech(QString name);
    void update();
    void init();

};

#endif // AUDIOSTECK_H

