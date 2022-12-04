#ifndef SPRITE_H
#define SPRITE_H

#include "src/defs.h"

class Sprite {
    public:
        Sprite(QPixmap spriteSheet, int row, int col, int len, bool directional = false, bool isPacman = false);
        ~Sprite();
        QPixmap get_next_phase(int offset = 0);
    private:
        QList<QPixmap> sprites{};
        QPoint location{};
        int phaseCount = 2;
        unsigned char index = 0;
        int phaseLen = 0;

};

#endif // SPRITE_H
