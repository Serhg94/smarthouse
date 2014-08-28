#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QtCore/QDebug>
#include <QSettings>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <QProcess>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QApplication *a, QWidget *parent = 0);
    QTimer *maintain_timer;
    ~MainWindow();

public slots:
    void initPortBox();
    void loadSettings();
    void saveSettings();
    void portButt();
    void alarmButt();
    void RefreshView(int sn, QString info);
    void appendStr(QString info);
    void maintain();

signals:
    void openPort(QString port);
    void alarmWindowOpen();

private:
    QApplication *app;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
