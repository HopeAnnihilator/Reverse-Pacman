#ifndef TITLESCREEN_H
#define TITLESCREEN_H

#include <QDialog>
#include "leaderboard.h"
#include "ui_titlescreen.h"

namespace Ui {
    class TitleScreen;
}

class TitleScreen : public QDialog
{
    Q_OBJECT

public:
    explicit TitleScreen(wsClient* socket, QWidget *parent = nullptr);
    ~TitleScreen();

private:
    Ui::TitleScreen *ui{};
    LeaderBoard* l{};
    wsClient* ws{};
    QString lobbyid{};

Q_SIGNALS:
    void start_game();

    void get_lobbies();
    void host_lobby();

private slots:
    void joined_lobby(QString);
     //void startGame();

};

#endif // TITLESCREEN_H
