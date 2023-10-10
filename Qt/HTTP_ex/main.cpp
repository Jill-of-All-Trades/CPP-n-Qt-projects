#include <QApplication>
#include "downloadergui.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DownloaderGui downloader;

    downloader.show();
    downloader.resize(280,120);

    return a.exec();
}
