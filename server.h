#ifndef SERVER_H
#define SERVER_H

#include <QTimer>
#include <QUdpSocket>
#include <QTcpServer>
#include <QTcpSocket>

class Server : public QObject
{
    Q_OBJECT

public:
    Server(quint16 listeningPort, quint16 broadcastPort);
    ~Server();
    void startListening();
    void stopListening();
    void sendMessage(QString message);
private:
    quint16 listeningPort;
    quint16 broadcastPort;
    QUdpSocket *broadcastUdpSocket;
    QTimer *broadcastTimer;
    QTcpServer *server;
    QTcpSocket *client;
private slots:
    void onSendBroadcastDatagram();
    void onAcceptConnection();
    void onStartRead();
    void onClientDisconnected();
signals:
    void log(QString message);
    void serverStatusUpdated(bool status);
    void clientStatusUpdated(bool status);
};

#endif // SERVER_H
