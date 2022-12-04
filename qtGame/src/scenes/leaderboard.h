#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <src/sockets/wsClient.h>
#include "ui_leaderboard.h"

#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

namespace Ui {
    class LeaderBoard;

}

class QLabel;

class LeaderBoard : public QDialog {
    Q_OBJECT
public:
    explicit LeaderBoard(wsClient* sock, QWidget *parent = nullptr);
    ~LeaderBoard();
    QGridLayout *gridLayout = new QGridLayout;

signals:


public slots:

    void get_leaderboard();
private slots:
    void show_leaderboard();
private:
    Ui::LeaderBoard *ui{};

    wsClient *ws{};
    QJsonArray scores{};
    QLabel* LeaderBaordTitle{};        //pointer to echo QLabel;

    QPushButton* goBack{}; // pointer to quit QPushButton
    QPushButton* quitButtonPtr{}; //pointer to clear QPushButton


};

#endif // LEADERBOARD_H
