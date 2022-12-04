#include "gameInterface.h"


GameInterface::GameInterface(QWidget *parent) : QMainWindow(parent), ui(new Ui::GameInterface) {

    ui->setupUi(this);

    scene = new QGraphicsScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setBackgroundBrush(Qt::black);

    // Make scene object the data source for the view object

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setFocusPolicy(Qt::StrongFocus);

    connect(ws, &wsClient::change_direction, this, &GameInterface::set_char);
    // Configure timer object to drive animation
    timer = new QTimer;
}

void GameInterface::start_game() {

    timer->setInterval(FRAME_DELAY);
    connect(timer, &QTimer::timeout, scene, &QGraphicsScene::advance);
    connect(timer, &QTimer::timeout, this, &GameInterface::update_score);
    drawMap();
    // add player objects after map so always on top of map
    QList<Player *>::const_iterator player;
    for (player = players.constBegin(); player != players.constEnd(); ++player) {
        scene->addItem((*player));
        (*player)->setVisible(false);
    }

    // add laser object after players
    scene->addItem(laser);
    connect(ws, &wsClient::change_direction, this, &GameInterface::move_char);
    connect(ws, &wsClient::laser_fired, this, &GameInterface::fire_laser);
    connect(ws, &wsClient::remove_player, this, &GameInterface::remove_player);
    connect(ws, &wsClient::add_player, this, &GameInterface::add_player);


}

void GameInterface::keyPressEvent(QKeyEvent *e) {
    if (e) {
        switch (e->key()) {
            case Qt::Key_A:
            case Qt::Key_J:
            case Qt::Key_Left:
                ws->send_movement(Directions::Left);
                break;

            case Qt::Key_D:
            case Qt::Key_L:
            case Qt::Key_Right:
                ws->send_movement(Directions::Right);
                break;

            case Qt::Key_W:
            case Qt::Key_I:
            case Qt::Key_Up:
                ws->send_movement(Directions::Up);
                break;

            case Qt::Key_S:
            case Qt::Key_K:
            case Qt::Key_Down:
                ws->send_movement(Directions::Down);
                break;
            case Qt::Key_Space:
                if (ws->whoami() == "pacman") {ws->send_laser();}
                if (!laser->isVisible()) {
                    laser->set_sprite(&sprites.find("laser").value());
                    players.back()->shootLaser();
                }
                break;
            case Qt::Key_T:
                ws->start_game();
                break;
            default:
                break;
        }
    }
}

void GameInterface::move_char(QString cName, int direction) {
    QList<Player *>::const_iterator player;
    for (player = players.constBegin(); player != players.constEnd(); ++player) {
        if ((*player)->get_cName() == cName) {
            (*player)->set_direction(static_cast<Directions> (direction));
        }
    }
}

void GameInterface::fire_laser() {
    QList<Player *>::const_iterator player;
    for (player = players.constBegin(); player != players.constEnd(); ++player) {
        if ((*player)->get_cName() == "pacman") {
            (*player)->shootLaser();
        }
    }
}

void GameInterface::addMapItem(int row, int col, QString objName, Map objType, GameObject* obj) {
    obj->set_ObjectType(objType);
    obj->setPos(col * SPRITE_DIMENSIONS, row * SPRITE_DIMENSIONS);
    if (objType != Map::Blank)
        obj->set_sprite(&sprites.find(objName).value());
    map.insert(QPoint(col, row), obj);
}
void GameInterface::spawn_players(int row, int col, QString charName, Map objectType) {
    Player* newPlayer = new Player();
    addMapItem(row, col, charName, objectType, newPlayer);
    newPlayer->set_map(&map);
    newPlayer->set_spawnPoint(row, col);
    newPlayer->set_laser(laser);
    newPlayer->set_char(charName);
    newPlayer->set_players(players);
    players.push_back(newPlayer);
}

void GameInterface::drawMap() {
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
       //throw std::exception("CANT LOAD MAP");
        emit close();

    // create laser
    laser = new Projectile();
    laser->set_sprite(&sprites.find("laser").value());
    laser->setVisible(false);
    laser->setPos(500, 500);
    laser->set_map(&map);

    for (int row = 0; row < MAP_HEIGHT; row++) {
        QString line = file->readLine();
        for (int col = 0; col < MAP_WIDTH; col++) {
            GameObject* obj = new GameObject();
            switch(line.at(col).unicode()) {
                case Map::Wall:
                    addMapItem(row, col, "wall", Map::Wall, obj);
                    scene->addItem(obj); break;
                case Map::Dot:
                    addMapItem(row, col, "dot", Map::Dot, obj); ++dotCount;
                    scene->addItem(obj);  break;
                case Map::Gate:
                    addMapItem(row, col, "blank", Map::Gate, obj); break;
                case Map::Blank:
                    addMapItem(row, col, "blank", Map::Blank, obj); break;
                case Map::Energizer:
                    addMapItem(row, col, "energizer", Map::Energizer, obj); ++dotCount;
                    scene->addItem(obj); break;
                case Map::Pacman:
                    delete obj;
                    spawn_players(row, col, "pacman", Map::Pacman); break;
                case Map::Ghost:
                    delete obj;
                    spawn_players(row, col, ghosts.at(++currGhost % ghosts.size()), Map::Ghost);
                    break;
                default:
                    break;

            }
        }
    }
    file->close();
}
void GameInterface::finish_setup(QJsonArray toShow) {
    QJsonArray::const_iterator cName;

    for (cName = toShow.constBegin(); cName != toShow.constEnd(); ++cName) add_player((*cName).toString());
    timer->start();

}
GameInterface::~GameInterface(){delete ui;}
void GameInterface::set_socket(wsClient* ws) {this->ws = ws;}
void GameInterface::playDeath(GameObject* obj, QString animation) {obj->set_sprite(&sprites.find(animation).value());}
void GameInterface::set_char(QString cName) {
    this->whoami = cName;
    ui->cName->setText(cName);
    QList<Player *>::const_iterator player;
    for (player = players.constBegin(); player != players.constEnd(); ++player) {
        if ((*player)->get_cName() == cName) {
            if (!(*player)->isVisible())
                (*player)->setVisible(true);
            (*player)->set_alive(true);
        }
    }
}
void GameInterface::update_score() {
    quint16 deadDots = 0;
    int score = 0;
    objectMap::const_iterator mapIter;
    for (mapIter = map.constBegin(); mapIter != map.constEnd(); ++mapIter) {
        if ((*mapIter)->get_objectType() == Map::Pacman) {
            if ((*mapIter)->isVisible()) {

                score = (*mapIter)->get_score();
                ui->score->setNum(score);
            } else {
                // make game over screen  - submit high scores
                timer->stop();
                if (whoami == "pacman") {
                    (*mapIter)->setVisible(false);
                    g->set_score((*mapIter)->get_score());
                    g->show();
                    connect(g, &gameover::submit_score, ws, &wsClient::submit_score);
                }
                emit close();
            }
        } else if (((*mapIter)->get_objectType() == Map::Dot) || ((*mapIter)->get_objectType() == Map::Energizer))
            if (!(*mapIter)->isVisible())
                deadDots++;
    }
    // make game over screen  - submit high scores
    if (deadDots == dotCount) {
        timer->stop();
        if  (whoami == "pacman") {
            timer->stop();

            g->set_score(score);
            g->show();
            connect(g, &gameover::submit_score, ws, &wsClient::submit_score);

        }
        emit close();
    }

}
// remove player from screen
void GameInterface::remove_player(QString cName) {
    QList<Player *>::const_iterator player;
    for (player = players.constBegin(); player != players.constEnd(); ++player) {
        if ((*player)->get_cName() == cName) {
            (*player)->setVisible(false);
            (*player)->set_alive(false);
        }
    }
}
// add player to screen
void GameInterface::add_player(QString cName) {
    QList<Player *>::const_iterator player;
    for (player = players.constBegin(); player != players.constEnd(); ++player) {
        if ((*player)->get_cName() == cName) {
            if (!(*player)->isVisible())
                (*player)->setVisible(true);
            (*player)->set_alive(true);
        }
    }
}
