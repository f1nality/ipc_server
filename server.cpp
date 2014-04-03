#include "server.h"

Server::Server(quint16 listeningPort, quint16 broadcastPort)
{
    this->listeningPort = listeningPort;
    this->broadcastPort = broadcastPort;
    broadcastUdpSocket = new QUdpSocket();
    broadcastTimer = new QTimer();
    server = new QTcpServer();
    client = NULL;

    connect(broadcastTimer, SIGNAL(timeout()), this, SLOT(onSendBroadcastDatagram()));
    connect(server, SIGNAL(newConnection()), this, SLOT(onAcceptConnection()));
}

Server::~Server()
{
    stopListening();

    delete broadcastUdpSocket;
    delete broadcastTimer;
    delete server;
}

void Server::startListening()
{
    if (server->isListening())
    {
        emit log("already listening");

        return;
    }

    broadcastTimer->start(1000);
    server->listen(QHostAddress::Any, listeningPort);

    emit log(QString("StartListening on port %1, broadcasting on port %2").arg(listeningPort).arg(broadcastPort));
    emit serverStatusUpdated(true);
}

void Server::stopListening()
{
    if (client != NULL)
    {
        client->close();
    }

    broadcastTimer->stop();
    server->close();

    emit log("StopListening");
    emit serverStatusUpdated(false);
}

void Server::onSendBroadcastDatagram()
{
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();

    for (int i = 0; i < interfaces.size(); i++)
    {
        QList<QNetworkAddressEntry> addresses = interfaces[i].addressEntries();

        for (int j = 0; j < addresses.size(); j++)
        {
            if ((addresses[j].ip().protocol() == QAbstractSocket::IPv4Protocol) && (addresses[j].broadcast().toString() != ""))
            {
                QByteArray datagram = "IPC_SERVER_BROADCAST_MESSAGE";
                broadcastUdpSocket->writeDatagram(datagram.data(), datagram.size(), addresses[j].broadcast(), broadcastPort);
            }
        }
    }
}

void Server::onAcceptConnection()
{
    if (client)
    {
        emit log("acceptConnection ignoring, already connected");

        return;
    }

    client = server->nextPendingConnection();

    connect(client, SIGNAL(readyRead()), this, SLOT(onStartRead()));
    connect(client, SIGNAL(disconnected()), this, SLOT(onClientDisconnected()));

    emit log("acceptConnection");
    emit clientStatusUpdated(true);
}

void Server::onStartRead()
{
    QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());

    if (!client)
    {
        return;
    }

    QByteArray line = client->readLine(1024);
    QString line_str = QString(line);

    emit log(QString("rcv: %1").arg(line_str));
}

void Server::onClientDisconnected()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());

    if (!client || client != this->client)
    {
        return;
    }

    this->client = NULL;

    emit log("clientDisconnected");
    emit clientStatusUpdated(false);
}

void Server::sendMessage(QString message)
{
    if (!client)
    {
        emit log("No connected client");

        return;
    }

    emit log(QString("onSendPushButtonClicked %1").arg(message));

    client->write(message.toLatin1());
}
