#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>

class Server : public QObject
{
    Q_OBJECT

public:
    Server();
    void startListening();
    void stopListening();
    void sendMessage(QString message);
private:
    QTcpServer *server;
    QTcpSocket* client;
private slots:
    void onAcceptConnection();
    void onStartRead();
    void onClientDisconnected();
signals:
    void log(QString message);
    void serverStatusUpdated(bool status);
    void clientStatusUpdated(bool status);
};

#endif // SERVER_H
