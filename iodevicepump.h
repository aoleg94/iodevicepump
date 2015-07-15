#ifndef IODEVICEPUMP_H
#define IODEVICEPUMP_H

#include <QObject>
#include <QIODevice>

class IODevicePump : public QObject
{
    Q_OBJECT
    QIODevice *src, *dst;
    bool working;
    qint64 buffer_size;
public:
    explicit IODevicePump(QObject *_parent = 0)
        : QObject(_parent), src(0), dst(0), working(false), buffer_size(1024*1024) {}
    explicit IODevicePump(QIODevice* s, QIODevice* d, QObject* _parent)
        : QObject(_parent), src(s), dst(d), working(false), buffer_size(1024*1024) {}
    void setSource(QIODevice* s) { src = s; }
    void setDestination(QIODevice* d) { dst = d; }
    void setBufferSize(qint64 b) { buffer_size = b; }
public slots:
    void go();
signals:
    void finished();
    void error();
private slots:
    void slotDoPump();
    void slotDoPump(qint64 b);
    void slotFinished();
};

#endif // IODEVICEPUMP_H
