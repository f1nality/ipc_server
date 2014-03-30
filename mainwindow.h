#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "server.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    Server *server;
    void log(QString message);
private slots:
    void onStartListeningPushButtonClicked();
    void onStopListeningPushButtonClicked();
    void onSendPushButtonClicked();
    void onServerLog(QString message);
    void onServerStatusUpdated(bool status);
    void onClientStatusUpdated(bool status);
};

#endif // MAINWINDOW_H