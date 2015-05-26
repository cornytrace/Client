#include "AddGame.h"
#include "ui_AddGame.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QProcess>
#include <QMessageBox>
#include <QDebug>

AddGame::AddGame(QWidget* parent, Database* db) :
    QDialog(parent),
    ui(new Ui::AddGame),
    db(db)
{
    ui->setupUi(this);
    this->setObjectName("addGameDialog");

    connect(ui->CancelButton,SIGNAL(clicked(bool)),this,SLOT(close())); //Close window when cancel is pressed.
}

void AddGame::on_NextButton_clicked() {
    if(ui->radioCustom->isChecked()){
        addCustomLocation();
    }
    else if(ui->radioSteam->isChecked()) {
        addSteamLibrary();
    }
}

void AddGame::addCustomLocation() {
    QString name = QInputDialog::getText(0, "Game Name", "Game Name:");

    if (name.trimmed() == "")
    {
        QMessageBox::critical(0, "Error", "You must specify a game name!");
        return;
    }

    QFileDialog exeDialog;
    exeDialog.setWindowTitle("Select Executable");
    exeDialog.setFileMode(QFileDialog::ExistingFile);

    if (exeDialog.exec())
    {
        QStringList files = exeDialog.selectedFiles();
        QString exe = files.at(0);
        #ifdef Q_WS_MACX
            //Get the binary from the app bundle
            QDir dir(file + "/Contents/MacOS");
            QStringList fileList = dir.entryList();
            file = dir.absoluteFilePath(fileList.at(2));// USUALLY this is the executable (after ., ..)
        #endif

        QFileDialog wdDialog; // Working Directory
        wdDialog.setWindowTitle("Select Working Directory");
        wdDialog.setFileMode(QFileDialog::DirectoryOnly);
        wdDialog.setDirectory(exeDialog.directory().absolutePath());

        if (wdDialog.exec())
        {
            QStringList dirs = wdDialog.selectedFiles();
            QString dir = dirs.at(0);

            qDebug() << "Adding game:" << name << exe << dir;
            db->addGame(name, dir, exe);
        }
    }
}

void AddGame::addSteamLibrary() {

}
