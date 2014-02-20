#include "dialogshowheaders.h"
#include "ui_dialogshowheaders.h"

#include <QDebug>
#include <QStringListModel>

DialogShowHeaders::DialogShowHeaders(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogShowHeaders)
{
    ui->setupUi(this);
    model = new QStringListModel();
}

DialogShowHeaders::~DialogShowHeaders()
{
    delete model;
    delete ui;
}

void DialogShowHeaders::setHeaders(const QStringList headers) {
    model->setStringList(headers);
    ui->listView->setModel(model);
    ui->listView->show();
}
