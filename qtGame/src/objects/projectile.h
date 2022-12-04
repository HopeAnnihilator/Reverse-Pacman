#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "gameObject.h"

class Projectile : public GameObject {
public:
    virtual void advance(int phase) override;
    void set_map(objectMap* map);
private:
    objectMap* map{};


};

#endif // PROJECTILE_H
