#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

#include "src/scenes/gameover.h"
#include "src/objects/player.h"

#include "ui_gameInterface.h"
#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>

#include <QKeyEvent>

#include <QPainterPath>
#include <QFile>
#include <QTextStream>

#include "src/sockets/wsClient.h"

namespace Ui {class GameInterface;}

class GameInterface : public QMainWindow {
    Q_OBJECT

public:
    explicit GameInterface(QWidget *parent = nullptr);
    ~GameInterface();
    void set_socket(wsClient* ws);
    void set_gameover(gameover *g) {this->g = g;}
protected:
    void keyPressEvent(QKeyEvent *e);
    //void mousePressEvent(QMouseEvent *e);

private:
    Ui::GameInterface *ui{};
    QGraphicsScene* scene = nullptr;
    QTimer* timer{};
    QList<Player *> players{};
    Projectile* laser{};
    wsClient *ws{};
    // modified sprite sheet from: https://tcrf.net/images/6/6b/Pac-Man_Comparable_Sprite_Sheet.png
    QPixmap spriteSheet = QPixmap(":/src/images/spriteMapScaled48x48.png");
    QList<QString> ghosts = {"red-ghost", "blue-ghost", "orange-ghost", "pink-ghost"};
    short currGhost = 0;
    quint16 dotCount = 0;
    QHash<QString, Sprite> sprites =  {
        {"pacman-dead", Sprite(spriteSheet, 0, 0, 12)},
        {"ghost-dead", Sprite(spriteSheet, 3, 0, 8, true)},
        {"red-ghost", Sprite(spriteSheet, 4, 0, 8, true)},
        {"pink-ghost", Sprite(spriteSheet, 5, 0, 8, true)},
        {"blue-ghost", Sprite(spriteSheet, 6, 0, 8, true)},
        {"orange-ghost", Sprite(spriteSheet, 7, 0, 8, true)},
        {"pacman", Sprite(spriteSheet, 8, 0, 8, true, true)},
        {"laser", Sprite(spriteSheet, 3, 8, 4, true)},
        {"laser-dead", Sprite(spriteSheet, 2, 9, 1)},
        {"wall", Sprite(spriteSheet, 1, 0, 1)},
        {"energizer", Sprite(spriteSheet, 2, 0, 1)},
        {"dot", Sprite(spriteSheet, 2, 1, 1)},
    };
    gameover *g{};
    QFile* file = new QFile(":/src/objects/map.txt");
    void addMapItem(int row, int col, QString objName, Map objType, GameObject* obj = new GameObject);
    void spawn_players(int row, int col, QString charName, Map objType);
    objectMap map{};
    QString whoami{};


public slots:
    void start_game();
    void finish_setup(QJsonArray toHide);
    void move_char(QString cName, int direction);
    void fire_laser();
    void set_char(QString cName);
private slots:
    void remove_player(QString cName);
    void add_player(QString cName);
    void update_score();
    void drawMap();
    void playDeath(GameObject* obj, QString animation);


};

#endif // GAMEINTERFACE_H
