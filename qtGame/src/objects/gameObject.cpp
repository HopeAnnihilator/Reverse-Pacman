#include "gameObject.h"

GameObject::GameObject(QGraphicsItem *parent) : QGraphicsItem(parent) {}
GameObject::~GameObject() {}

QRectF GameObject::boundingRect() const {return QRectF(0, 0, SPRITE_DIMENSIONS, SPRITE_DIMENSIONS);}
QPainterPath GameObject::shape() const {QPainterPath path; path.addRect(rect); return QPainterPath();}
void GameObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {painter->drawPixmap(rect, sprite->get_next_phase((int) direction));}

void GameObject::advance(int) {return;}
int GameObject::get_score() {return score;}


