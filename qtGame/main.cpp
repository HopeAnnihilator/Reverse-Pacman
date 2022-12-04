#include "src/game.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Game g;
    g.init_menu();
    QObject::connect(&g, &Game::closed, &a, &QCoreApplication::quit);
    return a.exec();
}
