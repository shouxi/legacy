#include <QApplication>

#include "peer.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    Peer peer;
    return app.exec();
}
