#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(this->width(),this->height());

    server = new Server();

    connect(ui->startListeningPushButton, SIGNAL(clicked()), this, SLOT(onStartListeningPushButtonClicked()));
    connect(ui->stopListeningPushButton, SIGNAL(clicked()), this, SLOT(onStopListeningPushButtonClicked()));
    connect(ui->clearPushButton, SIGNAL(clicked()), this, SLOT(onClearPushButtonClicked()));
    connect(ui->sendPushButton, SIGNAL(clicked()), this, SLOT(onSendPushButtonClicked()));

    connect(server, SIGNAL(log(QString)), this, SLOT(onServerLog(QString)));
    connect(server, SIGNAL(serverStatusUpdated(bool)), this, SLOT(onServerStatusUpdated(bool)));
    connect(server, SIGNAL(clientStatusUpdated(bool)), this, SLOT(onClientStatusUpdated(bool)));
}

MainWindow::~MainWindow()
{
    server->stopListening();

    delete ui;
}

void MainWindow::log(QString message)
{
    ui->consoleTextEdit->append(QString("%1").arg(message));
}

void MainWindow::onStartListeningPushButtonClicked()
{
    server->startListening();
}

void MainWindow::onStopListeningPushButtonClicked()
{
    server->stopListening();
}

void MainWindow::onClearPushButtonClicked()
{
    ui->consoleTextEdit->clear();
}

void MainWindow::onServerLog(QString message)
{
    log(message);
}

void MainWindow::onServerStatusUpdated(bool status)
{
    if (status)
    {
        ui->serverStatusLabel->setText("online");
        ui->serverStatusLabel->setStyleSheet("QLabel { color: green; }");
    }
    else
    {
        ui->serverStatusLabel->setText("offline");
        ui->serverStatusLabel->setStyleSheet("QLabel { color: red; }");
    }
}

void MainWindow::onClientStatusUpdated(bool status)
{
    if (status)
    {
        ui->clientStatusLabel->setText("online");
        ui->clientStatusLabel->setStyleSheet("QLabel { color: green; }");
    }
    else
    {
        ui->clientStatusLabel->setText("offline");
        ui->clientStatusLabel->setStyleSheet("QLabel { color: red; }");
    }
}

void MainWindow::onSendPushButtonClicked()
{
    server->sendMessage(ui->messageLineEdit->text());
}
