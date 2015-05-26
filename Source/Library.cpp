#include "Library.h"
#include "ui_Library.h"
#include "AddGame.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QProcess>
#include <QMessageBox>
#include <QDebug>

Library::Library(Database db) :
    QWidget(0),
    db(db),
    ui(new Ui::Library)
{
    ui->setupUi(this);
    this->setObjectName("libraryUI");
    runningProcess = new QProcess(this);
    processRunning = false;
    connect(runningProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(finished(int, QProcess::ExitStatus)));

    QList<Game> games = db.getGames();
    for (auto game : games)
    {
        qDebug() << game.id << game.gameName << game.gameDirectory << game.executablePath;
    }

    refreshGames();
}

Library::~Library()
{
    delete ui;
    delete runningProcess;
}

void Library::on_testLaunch_clicked()
{
    if (!processRunning)
    {
        Game game = db.getGameByName(ui->gameListWidget->item(ui->gameListWidget->currentRow())->text());
        runProcess(game.executablePath, game.gameDirectory);
    }
    else
    {
        QMessageBox messageBox;
        messageBox.setText("Error: an application is already running.");
        messageBox.exec();
    }
}

void Library::on_addGame_clicked()
{
    AddGame* addGame = new AddGame(this,&db);
    addGame->exec();
    refreshGames();
}

void Library::on_removeGame_clicked()
{
    db.removeGameByName(ui->gameListWidget->item(ui->gameListWidget->currentRow())->text());
    refreshGames();
}

void Library::runProcess(QString file, QString workingDirectory)
{
    // TODO: Implement some threading
    if (!processRunning)
    {
        qDebug() << "Launching:" << file << ", at" << workingDirectory;
        runningProcess->setWorkingDirectory(workingDirectory);
        runningProcess->setStandardErrorFile("error.txt");
        runningProcess->setStandardOutputFile("log.txt");
        runningProcess->start("\"" + file + "\"");
        runningProcess->waitForStarted();
        processRunning = true;
    }
}

void Library::refreshGames()
{
    ui->gameListWidget->clear();
    QList<Game> gameList = db.getGames();
    for (auto game : gameList)
    {
        ui->gameListWidget->addItem(game.gameName);
    }
}

void Library::finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    processRunning = false;
}
