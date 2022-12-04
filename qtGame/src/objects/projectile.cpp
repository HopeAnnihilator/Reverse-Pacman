#include "projectile.h"

static const inline bool can_move(QPointF nextPos, objectMap* map) {return map->find(calculate_hashKey(nextPos)).value()->get_objectType() != Map::Wall;}


void Projectile::advance(int phase) {
    if (!phase) return;
    if (!isVisible()) return;

    auto loc = pos();

    switch(direction) {
        case Right:
            loc.setX(loc.x() + SPRITE_DIMENSIONS);
            if (can_move(loc, map))
                moveBy(PROJECTILE_SPEED, 0);
            else
                setVisible(false);
            break;
        case Left:
            loc.setX(loc.x() - PROJECTILE_SPEED);
            if (can_move(loc, map))
                moveBy(-PROJECTILE_SPEED, 0);
            else
                setVisible(false);
            break;
        case Up:
            loc.setY(loc.y() - PROJECTILE_SPEED);
            if (can_move(loc, map))
                moveBy(0, -PROJECTILE_SPEED);
            else
                setVisible(false);
            break;
        case Down:
            loc.setY(loc.y() + SPRITE_DIMENSIONS);
            if (can_move(loc, map))
                moveBy(0, PROJECTILE_SPEED);
            else
                setVisible(false);
            break;
    }
}

void Projectile::set_map(objectMap* map) {this->map = map;}
