#ifndef GAMEOVER_H
#define GAMEOVER_H

#include <QWidget>

#include "ui_gameover.h"


namespace Ui {
class gameover;
}

class gameover : public QWidget
{
    Q_OBJECT

public:
    explicit gameover(QWidget *parent = nullptr);
    ~gameover();
    void set_score(int score);
private:
    Ui::gameover *ui;
    int score = 0;
private Q_SLOTS:
    void submitScore();

Q_SIGNALS:
    void submit_score(QString playername, int score);
};

#endif // GAMEOVER_H
