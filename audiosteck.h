#ifndef AUDIOSTECK_H
#define AUDIOSTECK_H

#include <QtMultimedia>
#include <QMediaPlayer>
#include <QVector>
#include <QTimer>

class audiosteck : public QObject
{
    Q_OBJECT
public:
    explicit audiosteck(QWidget *parent = 0);
    QTimer *maint;
    QMediaPlayer *player;
    QVector <QString> playlist;
signals:

public slots:
    void add(QString name);
    void update();
    void init();

};

#endif // AUDIOSTECK_H

