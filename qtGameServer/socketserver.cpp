#include "socketserver.h"

#include <QJsonDocument>
#include <QJsonArray>

#include "QDebug"

#include <QRandomGenerator>

#include <QDateTime>

#define SQLDRIVER "QSQLITE"
#define DBNAME "genericDatabase.db"

// start websocket server on port 42069
// can be reached at ws://localhost:42069
wsServer::wsServer(quint16 port, QObject *parent) : QObject(parent), ws(new QWebSocketServer(QStringLiteral("WS SERVER"), QWebSocketServer::NonSecureMode, this)) {
    // check websocket binds properly
    if (ws->listen(QHostAddress::Any, port)) {
        qDebug() << "Starting server on port " << port;
        // ensure database starts properly and is populated
        if (!initDb()) {
            // exit if database broken :(
            ws->close();
            return;
        }

        // establish socket rules
        connect(ws, &QWebSocketServer::newConnection, this, &wsServer::onNewConnection);
        connect(ws, &QWebSocketServer::closed, this, &wsServer::closed);
    } else {
        // exit if cant claim port
        qDebug() << "Unable to claim port: " << port;
        ws->close();
    }
}

// close active sessions and die
wsServer::~wsServer() {
    qDeleteAll(activeClients.begin(), activeClients.end());
    ws->close();
}

// handle new connections
void wsServer::onNewConnection() {
    QWebSocket *newConnection = ws->nextPendingConnection();
    if (!activeClients.length()) {
        isGame = false;
        characters = charactersBase;
    }
    if ((activeClients.length() >= MAX_CONNECTIONS) || isGame)
        newConnection->close();
    else {
        // add rules for new connection1
        connect(newConnection, &QWebSocket::textMessageReceived, this, &wsServer::processMsg);
        connect(newConnection, &QWebSocket::disconnected, this, &wsServer::onDisconnect);
        // keep connection in context
        activeClients << newConnection;



        //connect(newConnection, &QObject::objectNameChanged, this, &wsServer::whoami);
    }
}

// handle disconnects
void wsServer::onDisconnect() {
    QWebSocket *client = qobject_cast<QWebSocket *> (sender());
    if (client) {
        activeClients.removeAll(client);
        if (client->objectName().length() > 0) {
            characters.push_front(client->objectName());
            QList<QWebSocket *>::const_iterator player;
            for (player = activeClients.constBegin(); player != activeClients.constEnd(); ++player) {
                if ((*player)->isValid()) {
                    (*player)->sendTextMessage(QJsonDocument({{"args", client->objectName()}, {"type", "REMOVE-PLAYER"}}).toJson(QJsonDocument::Compact).toStdString().c_str());
                }
            }
        }
        client->deleteLater();
        client->close();
    }
    if (activeClients.length() < 1) {
        characters = charactersBase;
        QList<QWebSocket *>::const_iterator player;
        for (player = activeClients.constBegin(); player != activeClients.constEnd(); ++player) {
            if ((*player)->isValid()) {
                (*player)->disconnect();
            }
        }
        isGame = false;
    }
}

// handle message
void wsServer::processMsg(QString msg) {
    QWebSocket *establishedConnection = qobject_cast<QWebSocket *> (sender());
    // eval msg and send response
    QString response = evalMsg(msg);
    if (establishedConnection) {

        QList<QWebSocket *>::const_iterator player;
        for (player = activeClients.constBegin(); player != activeClients.constEnd(); ++player) {
            if ((*player)->isValid()) {
                (*player)->sendTextMessage(response);
            }
        }
    }
}

// eval command, in future will be in format
// {"command": "EXAMPLE-COMMAND", "args": "ARGUMENT"}
QString wsServer::evalMsg(QString message) {
    QWebSocket *client = qobject_cast<QWebSocket *> (sender());
    QJsonObject msg = QJsonDocument::fromJson(message.toUtf8()).object();
    switch(commands.indexOf(msg.find("command").value().toString())) {
        case 0:
            // return leaderboard
            return QJsonDocument(this->getLeaderboard()).toJson(QJsonDocument::Compact).toStdString().c_str();
            break;
        case 1:
            // ready up
            if (!isGame ) {
                client->setProperty("status", "READY");
                // assign character
                client->setObjectName(characters.front());
                characters.pop_front();

                whoami(client);
                return QJsonDocument({{"args", client->objectName()}, {"type", "READY-UP"}}).toJson(QJsonDocument::Compact).toStdString().c_str();
            }
            break;
        case 2:
            // start game
            if (client->objectName() == "pacman") {
                QList<QWebSocket *>::const_iterator player;
                for (player = activeClients.constBegin(); player != activeClients.constEnd(); ++player) {
                     if ((*player)->isValid() && !(*player)->objectName().length()) {
                         (*player)->disconnect();
                         (*player)->close();
                        //return QJsonDocument({{"args", "NOT-ALL-READY"}, {"type", "ERROR"}}).toJson(QJsonDocument::Compact).toStdString().c_str();
                        break;
                    }
                }

                isGame = true;
                QJsonArray toShow{};
                for (player = activeClients.constBegin(); player != activeClients.constEnd(); ++player) {
                    if ((*player)->isValid() && (*player)->objectName().length()) {
                        toShow.push_back((*player)->objectName());
                    }
                }
                return QJsonDocument({{"args", toShow}, {"type", "START-GAME"}}).toJson(QJsonDocument::Compact).toStdString().c_str();
            }
            return QJsonDocument({{"args", "CANT-START"}, {"type", "ERROR"}}).toJson(QJsonDocument::Compact).toStdString().c_str();
            break;
        case 3:
            // player movement
            return QJsonDocument({{"args", QJsonObject{{"cName", client->objectName()}, {"direction", msg.value("args")}}}, {"type", "PLAYER-MOVEMENT"}}).toJson(QJsonDocument::Compact).toStdString().c_str();
            break;
        case 4:
            submit_score(msg.value("args").toObject().value("pName").toString(), msg.value("args").toObject().value("score").toInt());
            return QJsonDocument({{"args", "NONE"}, {"type", "NONE"}}).toJson(QJsonDocument::Compact).toStdString().c_str();
            break;
        case 5:
            // laser fired
            if (client->objectName() == "pacman")
                return QJsonDocument({{"args", 0}, {"type", "LASER-FIRED"}}).toJson(QJsonDocument::Compact).toStdString().c_str();
            return QJsonDocument({{"args", "CANT FIRE LASER"}, {"type", "ERROR"}}).toJson(QJsonDocument::Compact).toStdString().c_str();
            break;
        default:
            // return error
            break;
    }
    return QJsonDocument({{"args", "BAD-BAD"}, {"type", "ERROR"}}).toJson(QJsonDocument::Compact).toStdString().c_str();
}

// get leaderboard info
QJsonObject wsServer::getLeaderboard() {
    QJsonArray arr;
    QSqlQuery query(db);
    query.exec("SELECT * FROM leaderboard ORDER BY score DESC LIMIT 10");
    while(query.next()) {
        arr.append(QJsonObject({{"name", query.value(1).toString()}, {"score", query.value(0).toInt()}}));
    }
    // return leaderboard as json object
    return {{"rankings", arr}, {"timestamp", QDateTime::currentDateTime().toMSecsSinceEpoch()}, {"type", "LEADERBOARD"}};
}

// initialize database
bool wsServer::initDb() {
    if (!QSqlDatabase::isDriverAvailable(SQLDRIVER)) {
        qDebug() << "Unable to load database driver";
        return false;
    }
    db = QSqlDatabase::addDatabase(SQLDRIVER);
    db.setDatabaseName(DBNAME);
    if (!db.open()) {
        qDebug() << "Unable to obtain database";
        return false;
    }
    // create tables if not already made
    QSqlQuery query(db);
    // drop open lobbies if exist
    //query.exec("DROP TABLE lobbies");
    query.exec("CREATE TABLE IF NOT EXISTS leaderboard (score INTEGER NOT NULL, username TEXT NOT NULL)");
    // clear any excess scores
    query.exec("DELETE FROM leaderboard WHERE score NOT IN (SELECT * FROM leaderboard ORDER BY score DESC LIMIT 10)");
    //query.exec("CREATE TABLE IF NOT EXISTS lobbies (players INTEGER NOT NULL, id TEXT NOT NULL PRIMARY KEY, sockets TEXT)");

    // check fi scoreboard empty
    query.exec("SELECT COUNT(*) FROM leaderboard");
    if (query.next())
        if (query.value(0).toInt() < 10)
            populateEmptyDb(query, 10 - query.value(0).toInt());
    return true;
}

// populate scoreboard with sample scores if empty
void wsServer::populateEmptyDb(QSqlQuery query, int count) {
    QList<QString> sampleNames = {"Thunder Chad", "Bob", "theLegend69", "null", "KLEE", "AI", "Steven", "IM OUT OF IDEAS", "sPoNgEbOb", "qwerty"};
    QList<int> sampleScores = {125, 155, 100, 50, 40, 35, 250, 110, 110, 120};
    for (int i = 0; i < count; i++) {
        query.prepare("INSERT INTO leaderboard (score, username) VALUES (?, ?)");
        query.addBindValue(sampleScores.at(i));
        query.addBindValue(sampleNames.at(i));
        query.exec();
    }
}

// let players know who they are
void wsServer::assignPlayers() {
    QList<QWebSocket *>::const_iterator player;
    for (player = activeClients.constBegin(); player != activeClients.constEnd(); ++player) {
        if ((*player)->isValid()) {
            if (!(*player)->objectName().length()) {
                (*player)->setObjectName(characters.front());
                characters.pop_front();
            } else if ((*player)->objectName() != "pacman") {
                characters.push_back((*player)->objectName());
                (*player)->setObjectName(characters.front());
                characters.pop_front();
            }
            whoami(*player);

        }
    }
}

void wsServer::submit_score(QString pName, int score) {

    // check if valid insert
    if ((score > 9973) || (pName.length() > 16)) return;

    bool ok = true;
    for (int i = 0; i < pName.length(); i++) {
        if ((pName.at(i) == '-') || (pName.at(i) == ' ') || (pName.at(i) == '_'))
            ok &= true;
        else if ((pName.at(i) >= 'A') && (pName.at(i) <= 'Z'))
            ok &= true;
        else if ((pName.at(i) >= 'a') && (pName.at(i) <= 'z'))
            ok &= true;
        else if ((pName.at(i) >= '0') && (pName.at(i) <= '9'))
            ok &= true;
        else
            ok &= false;
    }
    if (ok) {
        // insert score
        QSqlQuery query(db);
        query.prepare("INSERT INTO leaderboard (score, username) VALUES (?, ?)");
        query.addBindValue(score);
        query.addBindValue(pName);
        query.exec();
        // clear any excess scores
        query.exec("DELETE FROM leaderboard WHERE score NOT IN (SELECT * FROM leaderboard ORDER BY score DESC LIMIT 10)");
    }
}

void wsServer::whoami(QWebSocket* client) {client->sendTextMessage(QJsonDocument({{"args", client->objectName()}, {"type", "WHOAMI"}}).toJson(QJsonDocument::Compact).toStdString().c_str());}
