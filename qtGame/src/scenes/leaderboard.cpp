#include "leaderboard.h"

LeaderBoard::LeaderBoard(wsClient* sock, QWidget *parent) : QDialog(parent), ui(new Ui::LeaderBoard), ws(sock)  {
    ui->setupUi(this);
    this->setStyleSheet("background-color: black;");
}
void LeaderBoard::get_leaderboard() {

    disconnect(ws, &wsClient::leaderboardReady, this, &LeaderBoard::get_leaderboard);
    try {
        scores = ws->getLeaderboard();
        show_leaderboard();
    } catch (const std::length_error) {
        qDebug() << "Empty Leaderboard";
        connect(ws, &wsClient::leaderboardReady, this, &LeaderBoard::get_leaderboard);
    } catch (const std::runtime_error) {
        //qDebug() << "Leaderboard Updating";
        connect(ws, &wsClient::leaderboardReady, this, &LeaderBoard::get_leaderboard);
    }
}

LeaderBoard::~LeaderBoard() {}
void LeaderBoard::show_leaderboard() {


    QJsonArray::const_iterator score;
    int ind1 = 0;
    for(score = scores.constBegin(); score != scores.constEnd(); ++score) {

            QJsonObject info = (*score).toObject();
            qDebug() << "NAME: " << info.value("name").toString();
            qDebug() << "SCORE: " << info.value("score").toInt();

            QString s = info.value("name").toString();
            //ind1++;
            if(ind1 == 0)
            {
                ui->pos00->setText(s);
                ui->pos10->setText(QString::number(info.value("score").toInt()));
            }
            else if(ind1 == 1)
            {
                ui->pos01->setText(s);
                ui->pos11->setText(QString::number(info.value("score").toInt()));
            }
            else if(ind1 == 2)
            {
                ui->pos02->setText(s);
                ui->pos12->setText(QString::number(info.value("score").toInt()));
            }
            else if(ind1 == 3)
            {
                ui->pos03->setText(s);
                ui->pos13->setText(QString::number(info.value("score").toInt()));
            }
            else if(ind1 == 4)
            {
                ui->pos04->setText(s);
                ui->pos14->setText(QString::number(info.value("score").toInt()));
            }
            else if(ind1 == 5)
            {
                ui->pos05->setText(s);
                ui->pos15->setText(QString::number(info.value("score").toInt()));
            }
            else if(ind1 == 6)
            {
                ui->pos06->setText(s);
                ui->pos16->setText(QString::number(info.value("score").toInt()));
            }
            else if(ind1 == 7)
            {
                ui->pos07->setText(s);
                ui->pos17->setText(QString::number(info.value("score").toInt()));
            }
            else if(ind1 == 8)
            {
                ui->pos08->setText(s);
                ui->pos18->setText(QString::number(info.value("score").toInt()));
            }
            else if(ind1 == 9)
            {
                ui->pos09->setText(s);
                ui->pos19->setText(QString::number(info.value("score").toInt()));
            }

             ind1++;


    }
    this->show();




}
