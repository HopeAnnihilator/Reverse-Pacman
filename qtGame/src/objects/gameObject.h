#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "sprite.h"

class GameObject : public QGraphicsItem {
public:
    explicit GameObject(QGraphicsItem *parent = nullptr);
    virtual ~GameObject() override;
    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    virtual void advance(int phase) override;
    Map get_objectType() {return objectType;};
    //bool is_objectVisible() {return isVisible;};

private:
    friend class Player;
    friend class Projectile;
    int score = 0;
    QRect rect = QRect(0, 0, SPRITE_DIMENSIONS, SPRITE_DIMENSIONS);
    Sprite* sprite{};
    Map objectType{};
    Directions direction = Directions::Right;
public Q_SLOTS:
    int get_score();
    void set_sprite(Sprite* s) {sprite = s;};
    void set_direction(Directions newDirection) {direction = newDirection;};
    void set_ObjectType(Map objectType) {this->objectType = objectType;};
};

typedef QHash<QPoint, GameObject *> objectMap;

#endif // GAMEOBJECT_H

