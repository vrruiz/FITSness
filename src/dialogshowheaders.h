#ifndef DIALOGSHOWHEADERS_H
#define DIALOGSHOWHEADERS_H

#include <QDialog>
#include <QStringListModel>

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
    QStringListModel *model;
};

#endif // DIALOGSHOWHEADERS_H
