#include "math.h"
#include "fitsimage.h"

#include <QDebug>

FitsImage::FitsImage(QObject *parent) :
    QObject(parent)
{
    imageLoaded = false;
    headers.clear();
    imageData = NULL;
    stat.average = 0.0;
    stat.stdDev = 0.0;
    stat.min = 0.0;
    stat.max = 0.0;
    dimension = 0;
}

bool FitsImage::read(const QString path)
{
    fitsfile *fptr;  /* FITS file pointer */
    int status = 0;  /* CFITSIO status value MUST be initialized to zero! */
    int hdutype, naxis;
    long naxes[2], totpix = 0, fpixel[2];
    double *pix;
    int single = 0, nkeys, hdupos;
    char card[FLEN_CARD];   /* Standard string lengths defined in fitsio.h */


    // QString to char tip: http://stackoverflow.com/a/3408319
    char* file_name;
    std::string fname = path.toStdString();
    file_name = new char [fname.size()+1];
    strcpy(file_name, fname.c_str());

    // Open FITS file
    fits_open_image(&fptr, file_name, READONLY, &status);
    free(file_name);

    if (status) {
        printf("Error: Couldn't open image");
        return false;
    }

    // Generate statistics
    double sum = 0.0;
    double min = 0.0;
    double max = 0.0;
    double average = 0.0;
    double stddev = 0.0;

    // Init histogram values
    MonoHistogram histogram;
    for (int i = 0; i < HISTOGRAM_MONO_MAX; i++) {
        histogram.value[i] = 0;
    }

    // Read headers
    if (fits_get_hdu_type(fptr, &hdutype, &status) || hdutype != IMAGE_HDU) {
        printf("Error: this program only works on images, not tables\n");
        return false;
    }

    // Get FITS file dimensions
    fits_get_img_dim(fptr, &naxis, &status);
    fits_get_img_size(fptr, 2, naxes, &status);

    if (status || naxis != 2) {
        printf("Error: NAXIS = %d.  Only 2-D images are supported.\n", naxis);
        return false;
    }

    // Read FITS headers
    fits_get_hdu_num(fptr, &hdupos);  /* Get the current HDU position */
    while (!status) { /* Main loop through each extension */
        fits_get_hdrspace(fptr, &nkeys, NULL, &status); /* get # of keywords */
        for (int i = 1; i <= nkeys; i++) { /* Read and print each keywords */
            if (fits_read_record(fptr, i, card, &status)) break;
            headers.append(card);
        }
        if (single) break;  /* quit if only listing a single header */
        fits_movrel_hdu(fptr, 1, NULL, &status);  /* try to move to next HDU */
        hdupos++;
    }
    if (status == END_OF_FILE)  status = 0; /* Reset after normal error */

    if (status) fits_report_error(stderr, status); /* print any error message */

    // Read FITS image

    // Allocate memory
    double *image = new double[naxes[0] * naxes[1] * sizeof(double) + 1]; /* Array for the whole image */
    if (image == NULL) {
        printf("Memory allocation error\n");
        return false;
    }

    pix = new double[naxes[0] + 1]; /* memory for 1 row */

    if (pix == NULL) {
        free(image);
        printf("Memory allocation error\n");
        return false;
    }

    totpix = naxes[0] * naxes[1];
    fpixel[0] = 1;  /* read starting with first pixel in each row */

    /* process image one row at a time; increment row # in each loop */
    for (fpixel[1] = 1; fpixel[1] <= naxes[1]; fpixel[1]++)
    {
       /* give starting pixel coordinate and number of pixels to read */
       if (fits_read_pix(fptr, TDOUBLE, fpixel, naxes[0], 0, pix, 0, &status)) {
           free(image);
           free(pix);
           printf("Error reading image");
           return false;
       }

       for (int i = 0; i < naxes[0]; i++) {
           image[(fpixel[1] - 1) * naxes[0] + i] = pix[i];
           sum += pix[i];
           if (min > pix[i]) min = pix[i];
           if (max < pix[i]) max = pix[i];
           long value = pix[i];
           if (pix[i] <= HISTOGRAM_MONO_MAX && pix[i] >= 0) {
               histogram.value[value] += 1;
           }
       }
    }
    fits_close_file(fptr, &status); // Close FITS file
    free(pix); // Free memory

    average = sum / totpix;
    for (int y = 0; y < naxes[1]; y++) {
        for (int x = 0; x < naxes[0]; x++) {
            stddev += pow(average - image[y * naxes[0] + x], 2);
        }
    }
    stddev = sqrt(stddev / totpix);

    if (imageData != NULL) {
        free(imageData);
    }

    // Set object values
    this->path = path;
    monoHistogram = histogram;
    imageData = image;
    dimension = naxis;
    size.setWidth(naxes[0]);
    size.setHeight(naxes[1]);
    stat.min = min;
    stat.max = max;
    stat.average = average;
    stat.stdDev = stddev;
    imageLoaded = true;

    return true;
}

QImage FitsImage::toQImage(int imageLow, int imageHigh, unsigned char screenLow, unsigned char screenHigh)
{
    QImage image;

    if (!imageLoaded) {
        printf("Error: No source FITS");
        return image;
    }

    image = QImage(this->size, QImage::Format_RGB888);
    image.fill(Qt::black);

    for (int y = 0; y < this->size.height(); y++) {
        for (int x = 0; x < this->size.width(); x++) {
            int color;
            double pixel = imageData[y * this->size.width() + x];
            if (pixel < imageLow) {
                color = 0;
            } else if (pixel > imageHigh) {
                color = 255;
            } else {
               color = (pixel - imageLow) / (imageHigh - imageLow) * (screenHigh - screenLow) + screenLow;
            }
           image.setPixel(QPoint(x, y), qRgb(color,color,color));
        }
    }

    return image;
}

long FitsImage::pixel(long x, long y)
{
    double pix;

    if (!imageLoaded) {
        printf("Error: No image");
        return -1;
    }

    if (x < 0 || x > size.width() ||
        y < 0 || y > size.height()) {
        printf("Error: Mouse position out of image limits\n");
        return -1;
    }

    pix = imageData[y * size.width() + x];
    return pix;
}

FitsImage::~FitsImage()
{
    if (imageData) free(imageData);
}
