#ifndef DEFS_H
#define DEFS_H

#define SPRITE_DIMENSIONS 48

#define MAP_WIDTH 29
#define MAP_HEIGHT 20

#define SCENE_WIDTH MAP_WIDTH * SPRITE_DIMENSIONS
#define SCENE_HEIGHT MAP_HEIGHT * SPRITE_DIMENSIONS

// Time interval between frames in milliseconds
#define FRAME_DELAY 50
#define PLAYER_SPEED SPRITE_DIMENSIONS / 6
#define PROJECTILE_SPEED 3 * PLAYER_SPEED

#define SPRITE_COUNT 11

#define POWERUP_DURATION 5000

#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QGraphicsObject>
#include <QtGui/QPixmap>
#include <QtGui/QPainter>
#include <QGraphicsScene>

#include <QtCore/QObject>
#include <QtCore/QtDebug>
#include <QtCore/QList>
#include <QtCore/QString>

#include <QtCore/QTimer>
#include <QtCore/QPoint>
#include <QtCore/QHash>

#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>

#include <QtGui/QRegion>
#include <QtCore/QRect>

#include <QtWebSockets/QWebSocket>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#include <stdexcept>

#include <QDateTime>

enum Directions : int {Right, Left, Up, Down}; static const int DirectionCount = 4;
enum Map : char {Dot = '0', Wall = '1', Gate = '2', Blank = '3', Energizer = '4', Pacman = 'p', Ghost = 'g'};
enum Points : int {Dotupdategcc = 5, Energizerupdategcc = 20, Kill = 30};

// modified from https://stackoverflow.com/questions/24498993/cannot-use-qpoint-as-qhash-key/24499128#24499128
static const inline quint16 qHash(const QPoint& key) {return qHash(static_cast<quint16> (key.x ()) << 8 | key.y ());}

static const inline QPoint calculate_hashKey(QPointF pos) {return QPoint(pos.x() / SPRITE_DIMENSIONS, pos.y() / SPRITE_DIMENSIONS);}

#endif // DEFS_H
