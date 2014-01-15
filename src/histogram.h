#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QWidget>

const int HISTOGRAM_MONO_MAX = 65535;

typedef struct {
    long int red[256];
    long int green[256];
    long int blue[256];
} RgbHistogram;

typedef struct {
    long int value[HISTOGRAM_MONO_MAX+1];
} MonoHistogram;

typedef enum {
    HistogramMono = 0,
    HistogramRGB = 1
} HistogramMode;

class Histogram : public QWidget
{
    Q_OBJECT
public:
    explicit Histogram(QWidget *parent = 0);
    void setHistogram(RgbHistogram *new_histogram);
    void setHistogram(MonoHistogram *new_histogram);
    ~Histogram();

signals:

public slots:

protected:
    bool histogramSet;
    HistogramMode histogramMode;
    RgbHistogram rgbHistogram;
    MonoHistogram monoHistogram;
    MonoHistogram monoNormHistogram;

    void paintEvent(QPaintEvent *event);
};

#endif // HISTOGRAM_H
