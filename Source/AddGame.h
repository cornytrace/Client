#ifndef ADDGAME_H
#define ADDGAME_H

#include "Database.h"

#include <QDialog>

class Database;

namespace Ui
{
    class AddGame;
}

class AddGame : public QDialog
{
    Q_OBJECT

public:
    AddGame(QWidget* parent, Database* db);

private slots:
    void on_NextButton_clicked();

private:
    void addCustomLocation();
    void addSteamLibrary();

    Ui::AddGame* ui;
    Database* db;
};

#endif // ADDGAME_H
