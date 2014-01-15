#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <math.h>
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QFileDialog>

#define ZOOM_HALF 6.0

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    fitsImage(this)
{
    ui->setupUi(this);
    setWindowTitle("FITSness");
    addToolBar(Qt::LeftToolBarArea, ui->mainToolBar);
    showMaximized();

    zoomFactor = 1;
    imageLoaded = false;

    filePath = "/Users/rvr/archivos/astro/fits/20131207/m42-10s-guiado.fit";
    readAndShowImage(filePath);

    ui->image->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched != ui->image) return false;
    if (event->type() != QEvent::MouseMove) return false;
    const QMouseEvent* const me = static_cast<const QMouseEvent*>(event);
    //might want to check the buttons here
    const QPoint p = me->pos(); //...or ->globalPos();
    if (imageLoaded) {
        long x, y;
        x = p.x() / zoomFactor;
        y = p.y() / zoomFactor;
        if (x < 0 || x > image.size().width() ||
            y < 0 || y > image.size().height()) {
            // Mouse out of image borders
            return false;
        }

        // Show stats
        long counts = fitsImage.pixel(x,y);
        QRgb rgb = image.pixel(QPoint(x, y));
        QColor color(rgb);
        long color_value = color.red();
        ui->imageLabel->setText(QString("Size: %1 x %2\nX, Y: %3,%4\nCounts: %5\nColor: %6\n\n"
                                        "Min: %7\nMax: %8\nAverage: %9\nStd. dev: %10")
                                .arg(QString::number(fitsImage.size.width()))
                                .arg(QString::number(fitsImage.size.height()))
                                .arg(QString::number(x))
                                .arg(QString::number(y))
                                .arg(QString::number(counts))
                                .arg(QString::number(color_value))
                                .arg(QString::number(fitsImage.stat.min, 'f', 0))
                                .arg(QString::number(fitsImage.stat.max, 'f', 0))
                                .arg(QString::number(fitsImage.stat.average, 'f', 0))
                                .arg(QString::number(fitsImage.stat.stdDev, 'f', 0))
                                );
    }
    return false;
}

void MainWindow::readAndShowImage(const QString &path)
{
    if (fitsImage.read(path)) {
        imageLoaded = true;
        showImage();
        ui->statusBar->showMessage(filePath);
        ui->sliderMax->setValue(ui->sliderMax->maximum());
        ui->sliderMin->setValue(ui->sliderMin->minimum());
        ui->zoomSlider->setValue(ZOOM_HALF);
    } else {
        // TODO: Set to zero
    }
}

void MainWindow::showImage()
{
    if (!imageLoaded) return; // No image to display
    image = fitsImage.toQImage(ui->sliderMin->value(), ui->sliderMax->value());
    pixmap = QPixmap::fromImage(image);
    int width = pixmap.size().width() * zoomFactor;
    int height = pixmap.size().height() * zoomFactor;
    ui->image->setPixmap(pixmap.scaled(width, height));
    ui->image->adjustSize();
    ui->histogram->setHistogram(&fitsImage.monoHistogram);
}


void MainWindow::actionFileOpen()
{
    QString path = QFileDialog::getOpenFileName();
    if (!path.isEmpty()) {
        filePath = path;
        readAndShowImage(filePath);
    }
}

void MainWindow::actionSlideChange()
{
    showImage();
}

void MainWindow::actionZoom()
{
    int value = ui->zoomSlider->value();
    if (value >= ZOOM_HALF) {
        zoomFactor = value + 1 - ZOOM_HALF;
    } else if (value < ZOOM_HALF) {
        zoomFactor = 1.0 / 2.0 / (ZOOM_HALF - value);
    }
    showImage();
}
