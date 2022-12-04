#include "titlescreen.h"
//#include "src/scenes/GameInterface.h"

TitleScreen::TitleScreen(wsClient* sock, QWidget *parent) : QDialog(parent), ui(new Ui::TitleScreen), ws(sock) {
   ui->setupUi(this);
   this->setStyleSheet("background-color: black;");
    //r,g,b
   ui->HighScores->setStyleSheet("background-color: rgb(0, 0, 0); color: rgb(255, 255, 255)");
   //ui->PlayButton->setStyleSheet("background-color: rgb(0, 0, 0); color: rgb(255, 255, 255)");
   ui->exitButton->setStyleSheet("background-color: rgb(0, 0, 0); color: rgb(255, 255, 255)");
   ui->JoinButton->setStyleSheet("background-color: rgb(0, 0, 0); color: rgb(255, 255, 255)");


   l = new LeaderBoard(ws);

    //title screen button selection
    connect(ui->exitButton, &QPushButton::clicked, this, &QDialog::close);
    connect(ui->HighScores, &QPushButton::clicked, l, &LeaderBoard::get_leaderboard);

    connect(ui->JoinButton, &QPushButton::clicked, this, &TitleScreen::start_game);
    connect(ws, &wsClient::joinedLobby, this, &TitleScreen::joined_lobby);

}

TitleScreen::~TitleScreen() {delete ui;}

void TitleScreen::joined_lobby(QString id) {lobbyid = id;}
