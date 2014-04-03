#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(this->width(),this->height());

    server = new Server(10000, 10001);

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

    delete server;
    delete ui;
}

void MainWindow::setStatusLabelOnline(QLabel *label, bool status)
{
    QString text = (status) ? "online" : "offline";
    QString styleSheet = (status) ? "QLabel { color: green; }" : "QLabel { color: red; }";

    label->setText(text);
    label->setStyleSheet(styleSheet);
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
    setStatusLabelOnline(ui->serverStatusLabel, status);
}

void MainWindow::onClientStatusUpdated(bool status)
{
    setStatusLabelOnline(ui->clientStatusLabel, status);
}

void MainWindow::onSendPushButtonClicked()
{
    server->sendMessage(ui->messageLineEdit->text());
}
