#include "dialogshowheaders.h"
#include "ui_dialogshowheaders.h"

DialogShowHeaders::DialogShowHeaders(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogShowHeaders)
{
    ui->setupUi(this);
    // Add column titles
    model = new QStandardItemModel(0, 3, this);
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Property")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Value")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Comment")));
}

DialogShowHeaders::~DialogShowHeaders()
{
    delete ui;
}

void DialogShowHeaders::setHeaders(const QStringList headers) {
    // Loop over FITS headers
    for (int i = 0; i < headers.size(); i++)
    {
        // Split
        // Format: PROPERTY = 'VALUE' / COMMENT
        QList<QStandardItem *> items;
        QString header = headers.at(i);
        QStringList values1 = header.split("=");

        // PROPERTY header
        QStandardItem *itemProperty = new QStandardItem(values1.at(0).trimmed());
        items.append(itemProperty);

        // Only PROPERTY available?
        if (values1.size() < 2) continue;

        // Split 'VALUE' / COMMENT
        QStringList values2 = values1.at(1).split("/");
        if (values2.size() > 1)
        {
            // Show 'VALUE' and COMMENT
            QStandardItem *itemValue = new QStandardItem(values2.at(0).trimmed());
            items.append(itemValue);
            QStandardItem *itemComment = new QStandardItem(values2.at(1).trimmed());
            items.append(itemComment);
        } else {
            // Only 'VALUE' available
            QStandardItem *itemValue = new QStandardItem(values1.at(1).trimmed());
            items.append(itemValue);
        }
        model->appendRow(items);
    }
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
}
