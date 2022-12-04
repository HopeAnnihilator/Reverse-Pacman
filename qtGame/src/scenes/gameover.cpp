#include "gameover.h"


gameover::gameover(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gameover)
{
    ui->setupUi(this);
    connect(ui->submitScore, &QPushButton::clicked, this, &gameover::submitScore);
}

gameover::~gameover()
{
    delete ui;
}
void gameover::set_score(int score) {ui->score->setNum(score); this->score = score;}
void gameover::submitScore() {
    emit submit_score(ui->pName->text(), score);
    emit close();
}
