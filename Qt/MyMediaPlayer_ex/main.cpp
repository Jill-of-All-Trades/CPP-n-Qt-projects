#include <QApplication>
#include "soundplayer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SoundPlayer soundPlayer;

    soundPlayer.resize(320,75);
    soundPlayer.show();

    return a.exec();
}
