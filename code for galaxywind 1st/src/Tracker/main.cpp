#include <QApplication>
#include "tracker.h"


int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    Tracker tracker;
    return app.exec();
}
