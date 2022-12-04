#ifndef WSCLIENT_H
#define WSCLIENT_H

#include "src/defs.h"
#include "src/objects/player.h"

class wsClient : public QObject {
    Q_OBJECT
public:
    explicit wsClient(QString address = "ws://localhost:42069", QObject *parent = nullptr);
    QJsonArray getLeaderboard();

private:
    QList<QString> msgTypes = {"ERROR", "LEADERBOARD", "START-GAME", "PLAYER-MOVEMENT", "READY-UP", "LASER-FIRED", "REMOVE-PLAYER", "READY-UP", "WHOAMI"};

    QWebSocket* ws;
    QUrl url{};
    QJsonObject leaderboard{};
    QString cName{};
    void updateLeaderboard();

Q_SIGNALS:
    void closed();
    void leaderboardReady();
    void joinedLobby(QString);
    void gameBegin(QJsonArray toHide);
    void change_direction(QString cName, int direction);
    void cname_change(QString cName);
    void laser_fired();
    void remove_player(QString cName);
    void add_player(QString cName);
public Q_SLOTS:
    void printLeaderboard();
    void start_game();
    void ready_up();
    void send_movement(Directions direction);
    void send_laser();
    void submit_score(QString pName, int score);
    QString whoami();

private Q_SLOTS:
    void onConnected();
    void onMsg(QString message);

};

#endif // WSCLIENT_H
