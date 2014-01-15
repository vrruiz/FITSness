#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "cfitsio/fitsio.h"
#include "histogram.h"
#include "fitsimage.h"

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

protected:
    FitsImage fitsImage;
    QImage image;
    QPixmap pixmap;
    QString filePath;
    float zoomFactor;
    bool imageLoaded;

    void readAndShowImage(const QString &path);
    void showImage();
    bool eventFilter(QObject* watched, QEvent* event);

public slots:
    void actionFileOpen();
    void actionSlideChange();
    void actionZoom();
};

#endif // MAINWINDOW_H
