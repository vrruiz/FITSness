#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "cfitsio/fitsio.h"
#include "histogram.h"
#include "fitsimage.h"
#include "dialogshowheaders.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    DialogShowHeaders *dialogShowHeaders;

protected:
    FitsImage fitsImage;
    QImage image;
    QPixmap pixmap;
    QString filePath;
    float zoomFactor;
    bool imageLoaded;

    void readAndShowImage(const QString &path);
    void showImage();
    void showStats(long x, long y);
    bool eventFilter(QObject* watched, QEvent* event);

public slots:
    void actionFileOpen();
    void actionShowHeaders();
    void actionSlideChange();
    void actionZoom();
};

#endif // MAINWINDOW_H
