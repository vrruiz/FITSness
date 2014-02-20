#ifndef FITSIMAGE_H
#define FITSIMAGE_H

#include <QObject>

#include "cfitsio/fitsio.h"
#include "histogram.h"

typedef struct
{
    float min;
    float max;
    float average;
    float stdDev;
    float mode;
} ImageStatistics;

class FitsImage : public QObject
{
    Q_OBJECT
public:
    float statAverage;
    float statStdDev;
    float statMin;
    float statMax;
    int dimension;
    ImageStatistics stat;
    QString path;
    QStringList headers;
    QSize size;
    MonoHistogram monoHistogram;
    double *imageData;

    explicit FitsImage(QObject *parent = 0);
    // explicit FitsImage(const QString filePath, QObject *parent = 0);
    bool read(const QString path);
    QImage toQImage(int imageLow = 0, int imageHigh = 65535, unsigned char screenLow = 0, unsigned char screenHigh = 255);
    long pixel(long x, long y);
    ~FitsImage();

private:
    bool imageLoaded;

signals:

public slots:

};

#endif // FITSIMAGE_H
