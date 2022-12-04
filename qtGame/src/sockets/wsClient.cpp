#include "wsClient.h"

QT_USE_NAMESPACE

wsClient::wsClient(QString address, QObject *parent) : QObject(parent), url(QUrl(address)) {

    ws = new QWebSocket();
    connect(ws, &QWebSocket::connected, this, &wsClient::onConnected);

    connect(ws, &QWebSocket::disconnected, this, &wsClient::closed);
    connect(ws, &QWebSocket::textMessageReceived, this, &wsClient::onMsg);

    ws->open(url);
}

void wsClient::updateLeaderboard() {ws->sendTextMessage(QJsonDocument({{"command", "GET-LEADERBOARD"}, {"args", 0}}).toJson(QJsonDocument::Compact).toStdString().c_str());}
void wsClient::start_game() {ws->sendTextMessage(QJsonDocument({{"command", "START-GAME"}, {"args", 0}}).toJson(QJsonDocument::Compact).toStdString().c_str());}
void wsClient::ready_up() {ws->sendTextMessage(QJsonDocument({{"command", "READY-UP"}, {"args", 0}}).toJson(QJsonDocument::Compact).toStdString().c_str());}
void wsClient::send_movement(Directions direction) {ws->sendTextMessage(QJsonDocument({{"command", "PLAYER-MOVEMENT"}, {"args", direction}}).toJson(QJsonDocument::Compact).toStdString().c_str());}
void wsClient::send_laser() {ws->sendTextMessage(QJsonDocument({{"command", "FIRE-LASER"}, {"args", 0}}).toJson(QJsonDocument::Compact).toStdString().c_str());}
void wsClient::submit_score(QString pName, int score) {ws->sendTextMessage(QJsonDocument({{"args", QJsonObject{{"pName", pName}, {"score", score}}}, {"command", "SUBMIT-SCORE"}}).toJson(QJsonDocument::Compact).toStdString().c_str());}
QString wsClient::whoami() {return cName;}

void wsClient::onConnected() {updateLeaderboard();}

void wsClient::onMsg(QString message) {
    QJsonObject msg = QJsonDocument::fromJson(message.toUtf8()).object();
    //qDebug() << msg;
    //if (msg.isEmpty()) throw std::length_error("recieved empty msg");
    switch(msgTypes.indexOf(msg.value("type").toString())) {
        case 0:
            //qDebug() << "error";
            break;
        case 1:
            leaderboard = msg;
            emit leaderboardReady();
            break;
        case 2:
            //start game
            qDebug() << msg;
            emit gameBegin(msg.value("args").toArray());
            break;
        case 3:
            // player movement
            emit change_direction(msg.value("args").toObject().value("cName").toString(),  msg.value("args").toObject().value("direction").toInt());
            break;
        case 4:
            // cname
            emit add_player(msg.value("args").toString());
            //this->cName = msg.value("args").toString();
            //emit cname_change(this->cName);
            break;
        case 5:
            emit laser_fired();
            break;
        case 6:
            //remove player
            emit remove_player(msg.value("args").toString());
            break;
        case 7:
            emit add_player(msg.value("args").toString());
            break;
        case 8:
            // whami
            this->cName = msg.value("args").toString();
            emit cname_change(this->cName);
            break;
        default:
            qDebug() << msg;
            break;
    }
}

QJsonArray wsClient::getLeaderboard() {
    if (leaderboard.isEmpty()) {
        updateLeaderboard();
        throw std::length_error("empty leaderboard");
    } else if ((QDateTime::currentDateTime().toMSecsSinceEpoch() - leaderboard.value("timestamp").toDouble()) > (10 * 60 * 1000)) {
        updateLeaderboard();
        throw std::runtime_error("out of date leaderboard");
    } else {
        return leaderboard.value("rankings").toArray();
    }
}

void wsClient::printLeaderboard() {
    try {
        qDebug() << this->getLeaderboard();
    } catch (const std::length_error) {
        qDebug() << "Empty Leaderboard";
    } catch (const std::runtime_error) {
        qDebug() << "Leaderboard Updating";
    }
}


