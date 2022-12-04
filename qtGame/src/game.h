#ifndef GAME_H
#define GAME_H

#include "scenes/titlescreen.h"
#include "scenes/gameInterface.h"
#include "scenes/gameover.h"
#include "sockets/wsClient.h"

class Game : public QObject {
    Q_OBJECT
public:
    explicit Game(QObject *parent = nullptr);
    ~Game();
    void init_menu();
private:
    wsClient* ws;
    TitleScreen* t;
    GameInterface* g;
    gameover* go;

Q_SIGNALS:
    void closed();
};

#endif // GAME_H
