#include <QCoreApplication>
#include "iodevicepump.h"
#include <QFile>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QFile* fp = new QFile(argv[0]);
    fp->open(QIODevice::ReadOnly);
    QFile* fp2 = new QFile("target");
    fp2->open(QIODevice::WriteOnly);
    IODevicePump *dp = new IODevicePump(fp, fp2, &a);
    dp->setBufferSize(32);
    a.connect(dp, SIGNAL(finished()), SLOT(quit()));
    a.connect(dp, SIGNAL(error()), SLOT(quit()));
    dp->go();

    return a.exec();
}
