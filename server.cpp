#include "server.h"

#define PORT 10000

Server::Server()
{
    server = new QTcpServer();
    client = NULL;

    connect(server, SIGNAL(newConnection()), this, SLOT(onAcceptConnection()));
}

void Server::startListening()
{
    if (server->isListening())
    {
        emit log("already listening");

        return;
    }

    server->listen(QHostAddress::Any, PORT);

    emit log(QString("StartListening on port %1").arg(PORT));
    emit serverStatusUpdated(true);
}

void Server::stopListening()
{
    server->close();

    emit log("StopListening");
    emit serverStatusUpdated(false);
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
