#include "player.h"

static const inline bool can_move(QPointF nextPos, objectMap* map, Map playerType) {
    bool canMove = true;
    if (playerType != Map::Ghost)
        canMove &= map->find(calculate_hashKey(nextPos)).value()->get_objectType() != Map::Gate;
    canMove &= map->find(calculate_hashKey(nextPos)).value()->get_objectType() != Map::Wall;
    return canMove;
}

static inline GameObject* curr_tile(QPointF currPos, objectMap* map) {
    currPos.setX(currPos.x() +(SPRITE_DIMENSIONS / 2));
    currPos.setY(currPos.y() +(SPRITE_DIMENSIONS / 2));
    return map->find(calculate_hashKey(currPos)).value();
}

static const inline bool can_turn(QPointF currPos, Directions turnDirection, objectMap* map, Map playerType) {
    bool canTurn = false;
    if (!(static_cast<quint16> (currPos.x()) % SPRITE_DIMENSIONS) && !(static_cast<quint16> (currPos.y()) % SPRITE_DIMENSIONS)) {
        switch(turnDirection) {
            case Directions::Right:
                currPos.setX(currPos.x() + SPRITE_DIMENSIONS);
                canTurn = can_move(currPos, map, playerType);
                break;
            case Directions::Left:
                currPos.setX(currPos.x() - SPRITE_DIMENSIONS);
                canTurn = can_move(currPos, map, playerType);
                break;
            case Directions::Up:
                currPos.setY(currPos.y() - SPRITE_DIMENSIONS);
                canTurn = can_move(currPos, map, playerType);
                break;
            case Directions::Down:
                currPos.setY(currPos.y() + SPRITE_DIMENSIONS);
                canTurn = can_move(currPos, map, playerType);
                break;
        }
    }
    return canTurn;
}


void Player::advance(int phase) {
    if (!phase) return;
    auto loc = this->pos();
    if (powerUpTicks)
        --powerUpTicks;

    if (get_objectType() == Map::Pacman) {
        auto currTile = curr_tile(loc, map);

        QList<Player *>::const_iterator player;
        for (player = players.constBegin(); player != players.constEnd(); ++player) {
            if ((*player)->get_objectType() == Map::Ghost) {
                if ((*player)->isVisible() && (*player)->isAlive) {
                    int dx = abs(static_cast<int> (this->pos().x() - (*player)->pos().x()));
                    int dy = abs(static_cast<int> (this->pos().y() - (*player)->pos().y()));
                    if ((dx + dy) < SPRITE_DIMENSIONS)
                        this->hide();
                }
            }


        }


        if (currTile->isVisible() && isAlive) {
            switch(currTile->get_objectType()) {
                case Map::Dot:
                    score += Points::Dotupdategcc;
                    currTile->setVisible(false);
                    break;
                case Map::Energizer:
                    score += Points::Energizerupdategcc;
                    currTile->setVisible(false);
                    powerUpTicks += POWERUP_DURATION / FRAME_DELAY;
                    break;
                default: break;
            }
        }
    } else if (isAlive) {
        int dx = abs(static_cast<int> (this->pos().x() - laser->pos().x()));
        int dy = abs(static_cast<int> (this->pos().y() - laser->pos().y()));
        if ((dx + dy) < SPRITE_DIMENSIONS) {
            laser->hide();
            this->hide();
            isAlive = false;
            QList<Player *>::const_iterator player;
            for (player = players.constBegin(); player != players.constEnd(); ++player) {
                if ((*player)->get_objectType() == Map::Pacman) {
                    (*player)->add_points(Points::Kill);
                }
            }
        }
    }

    // attempt to change direction if needed
    if (direction != bufferTurn) {
        switch (direction) {
            case Directions::Right:
                if (bufferTurn == Directions::Left)
                    direction = bufferTurn;
                else if (can_turn(loc, bufferTurn, map, get_objectType()))
                    direction = bufferTurn;
                break;
            case Directions::Left:
                if (bufferTurn == Directions::Right)
                    direction = bufferTurn;
                else if (can_turn(loc, bufferTurn, map, get_objectType()))
                    direction = bufferTurn;
                break;
            case Directions::Up:
                if (bufferTurn == Directions::Down)
                    direction = bufferTurn;
                else if (can_turn(loc, bufferTurn, map, get_objectType()))

                    direction = bufferTurn;
                break;
            case Directions::Down:
                if (bufferTurn == Directions::Up)
                    direction = bufferTurn;
                else if (can_turn(loc, bufferTurn, map, get_objectType()))
                    direction = bufferTurn;
                break;
        }
    }

    // attempt to move player location
    switch(direction) {
        case Directions::Right:
            loc.setX(loc.x() + SPRITE_DIMENSIONS);
            if (can_move(loc, map, get_objectType()))
                moveBy(PLAYER_SPEED, 0);
            break;
        case Directions::Left:
            loc.setX(loc.x() - PLAYER_SPEED);
            if (can_move(loc, map, get_objectType()))
                moveBy(-PLAYER_SPEED, 0);
            break;
        case Directions::Up:
            loc.setY(loc.y() - PLAYER_SPEED);
            if (can_move(loc, map, get_objectType()))
                moveBy(0, -PLAYER_SPEED);
            break;
        case Directions::Down:
            loc.setY(loc.y() + SPRITE_DIMENSIONS);
            if (can_move(loc, map, get_objectType()))
                moveBy(0, PLAYER_SPEED);
            break;
    }


}

void Player::shootLaser() {
    if (laser->isVisible() || !powerUpTicks) return;
    laser->setPos(this->pos());
    laser->set_direction(direction);
    laser->setVisible(true);
}
void Player::set_map(objectMap* map) {this->map = map;}
void Player::set_laser(Projectile* laser) {this->laser = laser;}
void Player::set_char(QString cName) {this->charName = cName;}
QString Player::get_cName() {return charName;}

