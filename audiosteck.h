#ifndef AUDIOSTECK_H
#define AUDIOSTECK_H

#include <QThread>
#include <QtMultimedia>
#include <QMediaPlayer>
#include <QVector>
#include <QTimer>

class audiosteck : public QThread
{
    Q_OBJECT
public:
    explicit audiosteck(QWidget *parent = 0);
    ~audiosteck();
    QTimer *maint;
    QMediaPlayer *player;
    QVector <QString> playlist;
    void run();
signals:

public slots:
    void add(QString name);
    void update();

};

#endif // AUDIOSTECK_H

