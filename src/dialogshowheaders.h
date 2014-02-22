#ifndef DIALOGSHOWHEADERS_H
#define DIALOGSHOWHEADERS_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class DialogShowHeaders;
}

class DialogShowHeaders : public QDialog
{
    Q_OBJECT

public:
    explicit DialogShowHeaders(QWidget *parent = 0);
    void setHeaders(const QStringList headers);
    ~DialogShowHeaders();

private:
    Ui::DialogShowHeaders *ui;
    QStandardItemModel *model;
};

#endif // DIALOGSHOWHEADERS_H
