#include "iodevicepump.h"

void IODevicePump::go()
{
    if(working)
        return;
    if(!src || !dst || buffer_size <= 0)
        throw std::bad_exception();
    connect(src, SIGNAL(readyRead()), this, SLOT(slotDoPump()));
    connect(dst, SIGNAL(bytesWritten(qint64)), this, SLOT(slotDoPump(qint64)));
    connect(this, SIGNAL(error()), this, SLOT(deleteLater()));
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
    working = true;
    slotDoPump();
}

void IODevicePump::slotDoPump()
{
    slotDoPump(0);
}

void IODevicePump::slotDoPump(qint64)
{
    QByteArray arr;
    while(qint64 nb = src->bytesAvailable())
    {
        nb = qMin(nb, buffer_size);
        arr.resize(nb);
        nb = src->read(arr.data(), nb);
        if(nb < 0)
        {
            emit error();
            return;
        }
        nb = dst->write(arr);
        if(nb < 0)
        {
            emit error();
            return;
        }
        else if(nb == 0) // congestion
            return;
    }
    if(!src->isSequential() && src->atEnd())
        slotFinished();
}

void IODevicePump::slotFinished()
{
    src->close();
    dst->close();
    emit finished();
}
