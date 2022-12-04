#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#define MAX_CONNECTIONS 5

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>

#include <QJsonObject>

#include <QtSql>

#include <QHash>
#include <QList>

#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class wsServer : public QObject {
    Q_OBJECT
public:
    explicit wsServer(quint16 port = 42069, QObject *parent = nullptr);
    ~wsServer();

Q_SIGNALS:
    void closed();

private Q_SLOTS:
    void onNewConnection();
    void processMsg(QString message);
    void onDisconnect();

private:
    void submit_score(QString pName, int score);
    QWebSocketServer *ws{};
    QList<QWebSocket *> activeClients{};
    QList<QString> commands = {"GET-LEADERBOARD", "READY-UP", "START-GAME", "PLAYER-MOVEMENT", "SUBMIT-SCORE", "FIRE-LASER"};
    QSqlDatabase db{};
    QList<QString> charactersBase = {"pacman", "red-ghost", "pink-ghost", "blue-ghost", "orange-ghost"};
    QList<QString> characters = {"pacman", "red-ghost", "pink-ghost", "blue-ghost", "orange-ghost"};
    void assignPlayers();
    QString evalMsg(QString msg);
    QJsonObject getLeaderboard();
    bool initDb();
    void populateEmptyDb(QSqlQuery query, int count = 10);
    bool isGame = false;
    QRandomGenerator* rng = QRandomGenerator::system();
    void whoami(QWebSocket* client);
};

#endif //SOCKETSERVER_H
