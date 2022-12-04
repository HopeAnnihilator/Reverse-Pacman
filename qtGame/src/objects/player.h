#ifndef PLAYER_H
#define PLAYER_H

#include "gameObject.h"
#include "projectile.h"

class Player : public GameObject {
public:
    virtual void advance(int phase) override;
    void set_spawnPoint(int row, int col) {spawnPoint = QPoint(row, col);};
    void set_map(objectMap* map);
    void set_laser(Projectile* laser);
    void set_char(QString cName);

private:
    QPoint spawnPoint{};
    objectMap* map{};
    Directions bufferTurn = Directions::Right;
    Projectile* laser{};

    QString charName{};
    quint16 powerUpTicks= 0;
    QList<Player *> players;
    bool isAlive = true;
public Q_SLOTS:
    void set_players(QList<Player *> players) {this->players = players;}
    void disable_powerup();
    void goLeft() {bufferTurn = Directions::Left;}
    void goRight() {bufferTurn = Directions::Right;}
    void goUp() {bufferTurn = Directions::Up;}
    void goDown() {bufferTurn = Directions::Down;}
    void set_direction(Directions newDirection) {bufferTurn = newDirection;}
    void shootLaser();
    QString get_cName();

    QPoint get_spawnPoint() {return spawnPoint;}
    void add_points(Points toAdd) {score += toAdd;}
    void set_alive(bool isAlive) {this->isAlive = isAlive;}
};
#endif // PLAYER_H
