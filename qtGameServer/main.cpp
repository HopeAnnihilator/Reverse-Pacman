#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include "socketserver.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    wsServer *server = new wsServer();
    QObject::connect(server, &wsServer::closed, &a, &QCoreApplication::quit);

    return a.exec();
}
