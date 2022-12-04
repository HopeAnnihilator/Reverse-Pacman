#include "sprite.h"

Sprite::Sprite(QPixmap spriteSheet, int row, int col, int len, bool directional, bool isPacman)
    : location(col * SPRITE_DIMENSIONS, row * SPRITE_DIMENSIONS), phaseLen(len) {

    if (!directional) phaseCount = 1;
    else phaseCount = phaseLen / DirectionCount;
    for (int i = 0; i < len; i++) {
        auto sprite = spriteSheet.copy(location.x() + SPRITE_DIMENSIONS * i, location.y(), SPRITE_DIMENSIONS, SPRITE_DIMENSIONS);
        sprites.push_back(sprite);
    }
    if (isPacman) {
        auto baseSprite = spriteSheet.copy(0, 0, SPRITE_DIMENSIONS, SPRITE_DIMENSIONS);
        for (int i = 0; i < DirectionCount; i++) {
            sprites.insert((i * DirectionCount + phaseLen / DirectionCount), baseSprite);
            sprites.insert((i * DirectionCount + phaseLen / DirectionCount) + 1, spriteSheet.copy(location.x() + SPRITE_DIMENSIONS + SPRITE_DIMENSIONS * i * phaseCount, location.y(), SPRITE_DIMENSIONS, SPRITE_DIMENSIONS));
        }
        phaseCount += 2;
    }
};
Sprite::~Sprite() {}

// magic, only works if phaseCount factor of 2 and less than 257
QPixmap Sprite::get_next_phase(int offset) {return sprites.at((offset * phaseCount) + (++index % phaseCount));}
