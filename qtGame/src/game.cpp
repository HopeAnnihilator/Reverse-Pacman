#include "game.h"

Game::Game(QObject *parent): QObject(parent) {

    g = new GameInterface();
    ws = new wsClient();
    t = new TitleScreen(ws);
    go = new gameover();
}

Game::~Game() {}

void Game::init_menu() {
    connect(t, &TitleScreen::start_game, t, &TitleScreen::close);
    connect(t, &TitleScreen::start_game, g, &GameInterface::show);
    connect(t, &TitleScreen::start_game, ws, &wsClient::ready_up);
    g->set_socket(ws);
    g->start_game();
    g->set_gameover(go);
    connect(ws, &wsClient::cname_change, g, &GameInterface::set_char);
    connect(ws, &wsClient::closed, this, &Game::closed);
    connect(ws, &wsClient::gameBegin, g, &GameInterface::finish_setup);
    t->show();

}

