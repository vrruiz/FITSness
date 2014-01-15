#include <math.h>
#include <QPainter>
#include "histogram.h"

const int HISTOGRAM_HEIGHT = 50;
const int MAX_LEVELS = 255;

Histogram::Histogram(QWidget *parent) :
    QWidget(parent)
{
    histogramSet = false;
    histogramMode = HistogramMono;
}

void Histogram::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter qp(this);
    QPen pen(Qt::red, 1, Qt::SolidLine);
    qp.setPen(pen);
    qp.fillRect(this->rect(), Qt::black);
    if (histogramSet == false) return;
    switch (histogramMode) {
        case HistogramMono:
            long max = 0;
            for (int i = 0; i < HISTOGRAM_MONO_MAX; i++) {
                if (monoHistogram.value[i] > max) max = monoHistogram.value[i];
            }
            for (int i = 0; i < HISTOGRAM_MONO_MAX; i++) {
                // Paint RGB histograms
                double norm = pow(monoHistogram.value[i], 1.0/3.0) / pow(max, 1.0/3.0) * size().height();
                long normalized = long(norm);
                long x = i * this->width() / HISTOGRAM_MONO_MAX;
                qp.drawLine(x, size().height(), x, size().height() - normalized);
            }

            break;
    }
}

void Histogram::setHistogram(MonoHistogram *new_histogram) {
    for (int i = 0; i < HISTOGRAM_MONO_MAX; i++) {
        monoHistogram.value[i] = new_histogram->value[i];
    }

    histogramSet = true;
    repaint();
}

Histogram::~Histogram() {
}
